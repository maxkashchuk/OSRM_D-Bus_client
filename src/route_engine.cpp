#include "route_engine.h"

namespace RouteEngine
{
    std::string Engine::compute_route(double start_coord_longitude,
                double start_coord_latitude,
                double end_coord_longitude,
                double end_coord_latitude)
    {
        params.coordinates.clear();

        params.coordinates.push_back(
            {
                util::FloatLongitude
                {
                    start_coord_longitude
                },
                util::FloatLatitude
                {
                    start_coord_latitude
                }
            }
        );

        params.coordinates.push_back(
            {
                util::FloatLongitude
                {
                    end_coord_longitude
                },
                util::FloatLatitude
                {
                    end_coord_latitude
                }
            }
        );

        const auto status = osrm.Route(params, result);

        result_json = result.get<json::Object>();

        std::stringstream ss;

        osrm::util::json::render(ss, result_json);

        return ss.str();
    }

    ErrorCode Engine::dbus_init()
    {
        if(connection->request_name("route_engine.server",
                    DBUSCXX_NAME_FLAG_REPLACE_EXISTING ) !=
                    DBus::RequestNameResponse::PrimaryOwner) {
            ErrorCode::ERROR;
        }

        std::shared_ptr<DBus::Object> compute_route_object = connection->create_object("/route_engine/compute_route",
                    DBus::ThreadForCalling::DispatcherThread);

        compute_route_object->create_method<std::string(double start_coord_longitude,
                    double start_coord_latitude,
                    double end_coord_longitude,
                    double end_coord_latitude)>("route_engine.server", "compute_route",
                    sigc::mem_fun(*this, &Engine::compute_route));

        return ErrorCode::SUCCESS;
    }
    
    ErrorCode Engine::json_write()
    {
        try
        {
            std::ofstream myfile;

            myfile.open (OUTPUT_JSON_ALIAS);

            osrm::util::json::render(myfile, result_json);

            myfile.close();
        } catch(const std::exception& e)
        {
            return ErrorCode::ERROR;
        }

        return ErrorCode::SUCCESS;
    }
}

int main(int argc, const char *argv[])
{
    RouteEngine::Engine engine;

    if (engine.dbus_init() != RouteEngine::ErrorCode::SUCCESS) {
        std::cout << "Failed to initialize DBUS communication" << "\n";
    }

    std::cin.get();

    return 0;
}
