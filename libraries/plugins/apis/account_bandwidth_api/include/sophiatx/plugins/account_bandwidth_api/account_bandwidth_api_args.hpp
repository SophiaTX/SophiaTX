#ifndef SOPHIATX_ACCOUNT_BANDWIDTH_API_ARGS_HPP
#define SOPHIATX_ACCOUNT_BANDWIDTH_API_ARGS_HPP

#include <sophiatx/protocol/types.hpp>
#include <sophiatx/plugins/account_bandwidth_api/account_bandwidth_api_objects.hpp>

namespace sophiatx { namespace plugins { namespace account_bandwidth_api {

struct get_account_bandwidth_args
{
   protocol::account_name_type account;
};

struct get_account_bandwidth_return
{
   optional<account_bandwidth> bandwidth;
};

} } } // sophiatx::plugins::account_bandwidth_api

FC_REFLECT( sophiatx::plugins::account_bandwidth_api::get_account_bandwidth_args,
            (account) )

FC_REFLECT( sophiatx::plugins::account_bandwidth_api::get_account_bandwidth_return,
            (bandwidth) )

#endif //SOPHIATX_ACCOUNT_BANDWIDTH_API_ARGS_HPP
