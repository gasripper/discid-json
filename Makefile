discid: discid.c
	gcc discid.c -ldiscid -ljson-c -o discid
	strip discid

install: discid
	/usr/bin/install -m 755 discid /usr/local/bin/discid

clean:
	rm -f discid

deps-ubuntu:
	apt-get -yq install libdiscid-dev libdiscid0 libjson-c-dev libjson-c5