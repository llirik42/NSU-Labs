import argparse

from proxy import *
from proxy_logger.default_proxy_logger import DefaultProxyLogger
from proxy_logger.proxy_logger import ProxyLogger

if __name__ == '__main__':
    # curl vk.com --socks5 127.0.0.1:1080
    # curl vk.com --socks5-hostname 127.0.0.1:1080

    parser = argparse.ArgumentParser(prog='Socks5')
    parser.add_argument('-p', '--port', type=int, help='port of proxy', required=True)
    args = parser.parse_args()
    port: int = args.port

    logger: ProxyLogger = DefaultProxyLogger()

    proxy: Proxy = Proxy(port=port, logger=logger)
    proxy.launch()
