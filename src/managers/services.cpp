#include "services.h"

#include "configuration.h"

namespace bernd_box {

Services::Services() {
  ServiceGetters getters = getGetters();

  peripheral_controller_.setServices(getters);
  task_controller_.setServices(getters);
  task_removal_task_.setServices(getters);
  ota_updater_.setServices(getters);
}

std::shared_ptr<Network> Services::getNetwork() { return network_; }

void Services::setNetwork(std::shared_ptr<Network> network) {
  network_ = network;
}

std::shared_ptr<Server> Services::getServer() { return server_; }

void Services::setServer(std::shared_ptr<Server> server) { server_ = server; }

peripheral::PeripheralController& Services::getPeripheralController() {
  return peripheral_controller_;
}

tasks::TaskController& Services::getTaskController() {
  return task_controller_;
}

OtaUpdater& Services::getOtaUpdater() { return ota_updater_; }

Scheduler& Services::getScheduler() { return scheduler_; }

ServiceGetters Services::getGetters() {
  ServiceGetters getters(std::bind(&Services::getNetwork, this),
                         std::bind(&Services::getServer, this));
  return getters;
}

Scheduler Services::scheduler_{};

peripheral::PeripheralFactory Services::peripheral_factory_{};

peripheral::PeripheralController Services::peripheral_controller_{
    peripheral_factory_};

tasks::TaskFactory Services::task_factory_{scheduler_};

tasks::TaskController Services::task_controller_{scheduler_, task_factory_};

tasks::TaskRemovalTask Services::task_removal_task_{scheduler_};

OtaUpdater Services::ota_updater_{scheduler_};

}  // namespace bernd_box
