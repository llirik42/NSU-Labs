from typing import List

import dns.resolver


def get_dns_servers_ips() -> List[str]:
    return list(dns.resolver.Resolver().nameservers)
