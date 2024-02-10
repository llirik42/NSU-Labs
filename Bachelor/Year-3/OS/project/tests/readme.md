# Примеры URL

* http://ccfit.nsu.ru/ (server returns 200 by HTTP/1.0 and sends header "Content-Length)
* http://lib.ru/ (server returns 200 by HTTP/1.0 and doesn't send header "Content-Length")
* http://vk.com/ (server returns 301 by HTTP/1.0 and sends header "Content-Length")
* http://google.com/ (server returns 301 by HTTP/1.0, redirects to "http://www.google.com/" and sends header "Content-Length")
* http://www.google.com/ (server returns 200 by HTTP/1.0 and sends header "Content-Length")
* http://ccfit.nsu.ru/~rzheutskiy/test_files/200mb.dat (file of 200 MB, server returns 200 by HTTP/1.0 and sends header "Content-Length")
* http://static.kremlin.ru/media/events/video/ru/video_high/LJmJ5nrjhyCfVNDigS1CHdlmaG15G8cR.mp4 (file of 107 MB, server returns 200 by HTTP/1.0 and sends header "Content-Length")
* http://mirror.sitsa.com.ar/ubuntu-releases/jammy/ubuntu-22.04.3-desktop-amd64.iso (file of 4.7 GB, server returns 200 by HTTP/1.0 and sends header "Content-Length")

All these servers return 200 by HTTP/1.0 and send header "Content-Length"
* http://212.183.159.230/1GB.zip
* http://212.183.159.230/512MB.zip
* http://212.183.159.230/200MB.zip
* http://212.183.159.230/100MB.zip
* http://212.183.159.230/50MB.zip
* http://212.183.159.230/20MB.zip
* http://212.183.159.230/10MB.zip
* http://212.183.159.230/5MB.zip
