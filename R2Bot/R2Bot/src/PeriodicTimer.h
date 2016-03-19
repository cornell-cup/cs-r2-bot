#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>

namespace R2D2 {
	template <class Callable>
	class PeriodicTimer {
	public:
		PeriodicTimer(Callable& callable, unsigned int period, boost::asio::io_service& service);

	private:
		Callable& callable;
		unsigned int period;
		boost::asio::steady_timer timer;

		void reschedule();
	};


	// IMPLEMENTATION

	template<class Callable>
	PeriodicTimer<Callable>::PeriodicTimer(Callable& callable, unsigned int period, boost::asio::io_service& service)
		: callable(callable), period(period), timer(service)
	{
		reschedule();
	}

	template<class Callable>
	void PeriodicTimer<Callable>::reschedule() {
		timer.expires_from_now(std::chrono::milliseconds(period));
		timer.async_wait([this](const boost::system::error_code& e) {
			if (e != boost::asio::error::operation_aborted) {
				callable();
				reschedule();
			}
		});
	}
}