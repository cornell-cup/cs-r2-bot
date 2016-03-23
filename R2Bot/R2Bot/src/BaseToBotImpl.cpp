#include "StdAfx.h"
#include "BaseToBotImpl.h"
#include "Util.h"
#include <stdio.h>   
#include <stdlib.h>
#include <algorithm>

#define SOUNDS_DIR "sounds\\"

const float CM_PER_FT = 30.48f;

namespace R2D2 {

	BaseToBotImpl::BaseToBotImpl(
		//WheelController& wctrl,
		//HeadController& hctrl,
		uint64_t connectionTimeout,
		boost::asio::io_service& service)
		: 
		//wheelController(wctrl),
		//  headController(hctrl),
		  connectionTimeout(connectionTimeout),
		  lastPingTime(0),
		  pingTimer(service) { }

	void BaseToBotImpl::ping(const Ice::Current& current) {
		std::unique_lock<std::mutex> guard(this->lock);
		auto connInfo = Ice::TCPConnectionInfoPtr::dynamicCast(current.con->getInfo());
		// Ice gives the remote address as starting with "::ffff:", so we strip it off
		std::string hostname = connInfo->remoteAddress.substr(7, std::string::npos);

		if (std::find(hosts.begin(), hosts.end(), hostname) == hosts.end()) {
			hosts.push_back(hostname);
		}

		// We only track pings for the Base, which is host[0]
		if (hostname == hosts[0]) {
			// Reset ping timer
			pingTimer.expires_from_now(std::chrono::milliseconds(connectionTimeout));
			pingTimer.async_wait([this](const boost::system::error_code& e) {
				if (e != boost::asio::error::operation_aborted) {
					Log::warn("Lost Base connection; stopping motors");
					//wheelController.moveSpeed(0, 0);
				}
			});

			lastPingTime = timeSinceEpoch();
		}
	}
	
	void BaseToBotImpl::playSound(Sound snd, const Ice::Current&) {
		const char* file = NULL;
		switch (snd) {
		case ACKNOWLEDGE: file = SOUNDS_DIR "acknowledge1.wav"; break;
		case ANXIOUS:     file = SOUNDS_DIR "anxious.wav"; break;
		case CHEERFUL:    file = SOUNDS_DIR "cheerful.wav"; break;
		case EXCITED:     file = SOUNDS_DIR "excited1.wav"; break;
		case LAUGHING:    file = SOUNDS_DIR "laughing.wav"; break;
		//case SCREAM:      file = SOUNDS_DIR "scream2.wav"; break;
		case SAD:         file = SOUNDS_DIR "sad2.wav"; break;
		}
		PlaySound(file, NULL, SND_FILENAME | SND_ASYNC);
	}

	void BaseToBotImpl::move(int lSpeed, int rSpeed, const Ice::Current&) {
		Log::trace("move %d %d", lSpeed, rSpeed);
		//wheelController.moveSpeed(lSpeed, rSpeed);
	}

	void BaseToBotImpl::moveStraight(int dist, const Ice::Current&) {
		//wheelController.moveDist(dist, dist);
	}

	void BaseToBotImpl::turn(int angle, const Ice::Current&) {
		//wheelController.moveDist(-angle, angle);
	}

	void BaseToBotImpl::rotateHead(int angle, const Ice::Current&) {
		Log::trace("rotateHead %d", angle);
		//headController.moveHead(angle);
	}

	void BaseToBotImpl::setFlapState(FlapState state, const Ice::Current&) {
		Log::trace("setFlapState %d", state);
		//headController.moveFlap(state == FLAPOPEN);
	}

	void BaseToBotImpl::shutdown(const Ice::Current&){
		system("shutdown -s -t 1");
	}
	
	const std::vector<std::string>& BaseToBotImpl::getConnectedHosts() {
		return hosts;
	}

	bool BaseToBotImpl::isBaseConnected() {
		return (timeSinceEpoch() - lastPingTime) <= connectionTimeout;
	}
}