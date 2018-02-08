/*
 * controller.h
 *
 *  Created on: 08.02.2018
 *      Author: Simon
 */

#ifndef SRC_CONTROLLER_H_
#define SRC_CONTROLLER_H_

#include <thread>
#include <atomic>

#include "server-json-rpc.h"
#include "ui-handler.h"

namespace snapcaster {

class Controller {
public:
	Controller(const std::string& host, size_t port, const std::string& rotaryPath, const std::string& buttonPath);
	void start();
	void stop();

private:
	void worker();

	std::thread controllerThread;
	std::unique_ptr<ServerJsonRpc> serverJsonRpc;
	std::unique_ptr<UiHandler> uiHandler_;
};

} /* namespace snapcaster */

#endif /* SRC_CONTROLLER_H_ */
