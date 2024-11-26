#ifndef ROUTE_ENGINE_H
#define ROUTE_ENGINE_H

#include "osrm/match_parameters.hpp"
#include "osrm/nearest_parameters.hpp"
#include "osrm/route_parameters.hpp"
#include "osrm/table_parameters.hpp"
#include "osrm/trip_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/osrm.hpp"
#include "osrm/status.hpp"

#include "osrm/util/json_renderer.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <thread>
#include <chrono>

#include <cstdlib>

#include <fstream>

#include <dbus-cxx.h>

#define MAP_PATH "/home/maksym/Main/data/monaco-latest"
#define ALGORITHM EngineConfig::Algorithm::MLD
#define OUTPUT_JSON_PATH "../"
#define OUTPUT_JSON_ALIAS "polyline.json"

#ifdef __cplusplus
    extern "C"
    {
#endif

namespace RouteEngine
{
    using namespace osrm;

    enum ErrorCode
    {
        ERROR,
        SUCCESS
    };

    class Engine
    {
        private:
            EngineConfig config = {
                .storage_config = { MAP_PATH },
                .use_shared_memory = false,
                .algorithm = ALGORITHM
            };

            OSRM osrm{config};

            RouteParameters params;

            util::Coordinate coord_start, coord_end;

            engine::api::ResultT result
            {
                json::Object()
            };

            osrm::util::json::Object result_json;

            std::shared_ptr<DBus::Dispatcher> dispatcher
            {
                DBus::StandaloneDispatcher::create()
            };

            std::shared_ptr<DBus::Connection> connection
            {
                dispatcher->create_connection(DBus::BusType::SESSION)
            };

        private:
            std::string compute_route(double start_coord_longitude,
                double start_coord_latitude,
                double end_coord_longitude,
                double end_coord_latitude);

            ErrorCode dbus_read(osrm::util::json::Object result_json);

            ErrorCode dbus_write(osrm::util::json::Object result_json);

            ErrorCode json_write();

        public:
            ErrorCode dbus_init();
    };
}

#ifdef __cplusplus
    } // extern "C"
#endif

#endif // end