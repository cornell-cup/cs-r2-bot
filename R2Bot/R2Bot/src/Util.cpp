#include "stdafx.h"
#include "Util.h"

namespace R2D2 {

	uint64_t timeSinceEpoch() {
		using namespace boost::posix_time;
		ptime epoch(boost::gregorian::date(1970, 1, 1)); 
		ptime now = microsec_clock::local_time();
		return (now - epoch).total_milliseconds();
	}

}