import argparse

from proxy import *

if __name__ == '__main__':
    # curl vk.com --socks5 127.0.0.1:1080
    # curl vk.com --socks5-hostname 127.0.0.1:1080

    parser = argparse.ArgumentParser(prog='Socks5')
    parser.add_argument('-p', '--port', type=int, help='port of proxy', required=True)
    args = parser.parse_args()
    port: int = args.port

    proxy: Proxy = Proxy(port)
    proxy.launch()
