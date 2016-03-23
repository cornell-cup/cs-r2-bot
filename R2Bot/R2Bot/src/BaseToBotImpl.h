#pragma once
#include "R2Comm.h"
#include "WheelController.h"
#include "HeadController.h"
#include <string>
#include <mutex>
#include <condition_variable>

#include <boost/asio/steady_timer.hpp>

namespace R2D2 {

	class BaseToBotImpl : public BaseToBot {
	public:
		BaseToBotImpl(
			//WheelController& wctrl, HeadController& hctrl,
			uint64_t connectionTimeout, boost::asio::io_service& service);
		virtual void ping(const Ice::Current& current);
		virtual void playSound(Sound snd, const Ice::Current&);
		virtual void move(int lSpeed, int rSpeed, const Ice::Current&);
		virtual void rotateHead(int angle, const Ice::Current&);
		virtual void moveStraight(int dist, const Ice::Current&);
		virtual void turn(int angle, const Ice::Current&);
		virtual void setFlapState(FlapState state, const Ice::Current&);
		virtual void shutdown(const Ice::Current&);
		const std::vector<std::string>& getConnectedHosts();
		bool isBaseConnected();

	private:
		// R2D2 wheel controls
		//WheelController& wheelController;
		// R2D2 head controls
		//HeadController& headController;
		// Max time in milliseconds before we consider the connection dead
		uint64_t connectionTimeout;
		// Last time we got a ping from base
		uint64_t lastPingTime;
		// List of connected hosts, with Base at the head
		std::vector<std::string> hosts;
		// Lock & condition var used to signal connection
		std::mutex lock;
		std::condition_variable connectionCond;
		// Timer for pings
		boost::asio::steady_timer pingTimer;

		//VisionData data;  (can't locate VisionData)
	};

}