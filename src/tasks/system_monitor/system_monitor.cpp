#include "system_monitor.h"

namespace bernd_box {
namespace tasks {
namespace system_monitor {

SystemMonitor::SystemMonitor(const ServiceGetters& services,
                             Scheduler& scheduler)
    : BaseTask(scheduler), scheduler_(scheduler), services_(services) {
  setIterations(TASK_FOREVER);
  Task::setInterval(std::chrono::milliseconds(default_interval_).count());
}

SystemMonitor::~SystemMonitor() {}

const String& SystemMonitor::getType() const { return type(); }

const String& SystemMonitor::type() {
  static const String name{"SystemMonitor"};
  return name;
}

void SystemMonitor::SetInterval(std::chrono::milliseconds interval) {
  Task::setInterval(interval.count());
}

bool SystemMonitor::OnTaskEnable() {
  server_ = services_.getServer();
  if (server_ == nullptr) {
    setInvalid(services_.network_nullptr_error_);
    return false;
  }
  // Reset counters to calculate CPU load. Wait one interval for valid readings
  scheduler_.cpuLoadReset();
  delay();

  return true;
}

bool SystemMonitor::TaskCallback() {
  // Get the total available memory and largest continguous memory block.
  // This allows us to calculate the fragmentation index =
  //     (total free - largest free block) / total free * 100
  size_t free_bytes = heap_caps_get_free_size(MALLOC_CAP_8BIT);
  size_t max_malloc_bytes = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
  size_t least_free_bytes = heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT);

  DynamicJsonDocument doc(BB_JSON_PAYLOAD_SIZE);
  doc["free_memory_bytes"] = free_bytes;
  doc["heap_fragmentation_percent"] =
      (float(free_bytes) - float(max_malloc_bytes)) / float(free_bytes) *
      float(100);
  doc["least_free_bytes"] = least_free_bytes;

  float cpuTotal = scheduler_.getCpuLoadTotal();
  float cpuCycles = scheduler_.getCpuLoadCycle();
  float cpuIdle = scheduler_.getCpuLoadIdle();
  scheduler_.cpuLoadReset();

  // Productive work (not idle, not scheduling) --> time in task callbacks
  doc["productive_percent"] = 100 - ((cpuIdle + cpuCycles) / cpuTotal * 100.0);
  doc["wifi_rssi"] = WiFi.RSSI();

  server_->sendSystem(doc.as<JsonObject>());
  return true;
}

const std::chrono::seconds SystemMonitor::default_interval_{60};

}  // namespace system_monitor
}  // namespace tasks
}  // namespace bernd_box
