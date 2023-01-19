#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>

#include <nopayloadclient/backend.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/plmover.hpp>
#include <nopayloadclient/exception.hpp>


namespace nopayloadclient {

    // Reading
    nlohmann::json get(std::string gt_name, std::string pl_type,
                       long long major_iov, long long minor_iov);

    // Writing
    nlohmann::json createPayloadType(std::string pl_type);
    nlohmann::json createGlobalTag(std::string gt_name);
    nlohmann::json deleteGlobalTag(std::string gt_name);
    nlohmann::json lockGlobalTag(std::string gt_name);
    nlohmann::json unlockGlobalTag(std::string gt_name);
    nlohmann::json insertPayload(std::string gt_name, std::string pl_type, std::string file_url,
                                 long long major_iov_start, long long minor_iov_start);
    nlohmann::json insertPayload(std::string gt_name, std::string pl_type, std::string file_url,
                                 long long major_iov_start, long long minor_iov_start,
                                 long long major_iov_end, long long minor_iov_end);

   // Helper (Read-only)
   nlohmann::json getSize();
   nlohmann::json getPayloadTypes();
   nlohmann::json getGlobalTags();
   nlohmann::json checkConnection();
   nlohmann::json getConfDict();
   void sayHello();


   // Response creation (non-interface)
   template <class> struct ResponseDecorator;
   template <class R, class... Args>
   struct ResponseDecorator<R(Args ...)>
   {
      ResponseDecorator(std::function<R(Args ...)> f) : f_(f) {}

      nlohmann::json operator()(Args ... args)
      {
         try {
             return nlohmann::json::object({{"code", 0}, {"msg", f_(args...)}});
         }
         catch (BaseException &e) {
             return e.jsonify();
         }
      }
      std::function<R(Args ...)> f_;
   };

   template<class R, class... Args>
   ResponseDecorator<R(Args...)> makeResponse(R (*f)(Args ...))
   {
      return ResponseDecorator<R(Args...)>(std::function<R(Args...)>(f));
   }

}
