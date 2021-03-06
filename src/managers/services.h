#pragma once

#include <TaskSchedulerDeclarations.h>

#include "managers/service_getters.h"
#include "managers/network.h"
#include "managers/server.h"
#include "managers/ota_updater.h"
#include "peripheral/peripheral_controller.h"
#include "peripheral/peripheral_factory.h"
#include "tasks/task_controller.h"
#include "tasks/task_factory.h"
#include "tasks/task_removal_task.h"

namespace bernd_box {

/**
 * All the system services
 *
 * The services cover functions such as the connection to the server,
 * the peripheral and task controller as well as the scheduler. The task and
 * peripheral services are static instances, while the network and server are
 * created by the setup procedure setupNode().
 */
class Services {
 public:
  /**
   * Set the service getters for the static services (task and peripheral
   * controller as well as task removal task).
   */
  Services();
  virtual ~Services() = default;

  std::shared_ptr<Network> getNetwork();
  void setNetwork(std::shared_ptr<Network> network);

  std::shared_ptr<Server> getServer();
  void setServer(std::shared_ptr<Server> server);

  static peripheral::PeripheralController& getPeripheralController();
  static tasks::TaskController& getTaskController();
  static OtaUpdater& getOtaUpdater();

  static Scheduler& getScheduler();

  /**
   * Get callbacks to get the pointers to dynamic services (network and server)
   * 
   * \return Struct with callbacks to get pointers to the services
   */
  ServiceGetters getGetters();

 private:
  /// Handles network connectivity and time synchronization
  std::shared_ptr<Network> network_;
  /// Handles communication to the server
  std::shared_ptr<Server> server_;
  /// Executes the active tasks
  static Scheduler scheduler_;
  /// Creates peripherals with the registered peripheral factory callbacks
  static peripheral::PeripheralFactory peripheral_factory_;
  /// Handles server requests to create / delete peripherals
  static peripheral::PeripheralController peripheral_controller_;
  /// Creates tasks with the registered task factory callbacks
  static tasks::TaskFactory task_factory_;
  /// Handles server requests to start / stop tasks
  static tasks::TaskController task_controller_;
  /// Singleton to delete stopped tasks and inform the server
  static tasks::TaskRemovalTask task_removal_task_;
  /// Singleton to perform OTA updates
  static OtaUpdater ota_updater_;
};

}  // namespace bernd_box
