#include "Bbus.h"

Bbus::Bbus(){
	bus_traffic = 0;
}

std::vector<int> Bbus::BusRd(uint32_t tag, uint32_t set_number, uint32_t index){
	bus_traffic ++;
	std::vector<int> ret;
	for (size_t i = 0; i < bus.size(); ++i){
		if (i == index){
			continue;
		}
		ret.push_back(bus[i]->BusRd(tag, set_number));
	}
	return  ret;
}

std::vector<int> Bbus::BusRdX(uint32_t tag, uint32_t set_number, uint32_t index){
	bus_traffic ++;
	std::vector<int> ret;
	for (size_t i = 0; i < bus.size(); ++i){
		if (i == index){
			continue;
		}
		ret.push_back(bus[i]->BusRdX(tag, set_number));
	}
	return ret;
}


void Bbus::BusUpgr(uint32_t tag, uint32_t set_number){
	bus_traffic ++;
	for(auto & cache : bus){
		cache->BusUpgr(tag, set_number);
	}
}

void Bbus::Flush(uint32_t tag, uint32_t set_number){
	bus_traffic ++;
	for(auto & cache : bus){
		cache->Flush(tag, set_number);
	}
}

uint32_t Bbus::add_cache(Cache* cache){
	bus.push_back(cache);
	return bus.size()-1;
}

std::vector<Cache*> Bbus::caches(){
	return bus;
}
