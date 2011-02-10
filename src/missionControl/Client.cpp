#include "Client.hpp"

#include <boost/asio.hpp>

using namespace kocmoc::missionControl;

MissionControlClient::MissionControlClient()
{
	cout<<"starting mission control client" << endl;
	// create socket connection to server
	// send 'ping'
}
