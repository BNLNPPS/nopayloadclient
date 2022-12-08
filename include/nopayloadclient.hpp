#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>

#include <backend.hpp>
#include <payload.hpp>
#include <plmover.hpp>
#include <exception.hpp>


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


   // Response creation (non-interface)
   template <class> struct ResponseDecorator;
   template <class R, class... Args>
   struct ResponseDecorator<R(Args ...)>
   {
      ResponseDecorator(std::function<R(Args ...)> f) : f_(f) {}

      nlohmann::json operator()(Args ... args)
      {
         std::cout << "Calling the decorated function.\n";
         try {
             return nlohmann::json::object({{"code", 0}, {"msg", f_(args...)}});
         }
         catch (NoPayloadException &e) {
             return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
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
