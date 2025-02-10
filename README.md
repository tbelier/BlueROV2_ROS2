# BlueROV2_ROS2
This repo allows me to store my ROS2 scripts to manipulate my BlueROV2 named Castor and Pollux.


## Pour avoir accès à l'USBL


udp-connect:<host>:<port>	groups=FD,SOCKET,IP4,IP6,UDP
      udp-datagram:<host>:<port>	groups=FD,SOCKET,RANGE,IP4,IP6,UDP
      udp-listen:<port>	groups=FD,SOCKET,LISTEN,CHILD,RANGE,IP4,IP6,UDP
      udp-recv:<port>	groups=FD,SOCKET,RANGE,IP4,IP6,UDP
      udp-recvfrom:<port>	groups=FD,SOCKET,CHILD,RANGE,IP4,IP6,UDP
      udp-sendto:<host>:<port>	groups=FD,SOCKET,IP4,IP6,UDP
      
      
      
 ### SUR LA RPI via le terminal blueos.local:
 
	Crée un fichier socat_usb0.sh dans /usr/local/bin/ :

		sudo nano /usr/local/bin/socat_usb0.sh

	Ajoute ce code :

		#!/bin/bash
		while true; do
		    socat -d -d TCP-LISTEN:22222,reuseaddr FILE:/dev/ttyUSB0,raw,echo=0
		    sleep 1  # Petit délai avant de relancer en cas d'erreur
		done

	Enregistre et ferme.

	Puis, rends le script exécutable :

		sudo chmod +x /usr/local/bin/socat_usb0.sh
	 
	Ensuite pour lancer sur le bluerov la connexion udp au démarrage du rov : 


	Édite le crontab root :

		sudo crontab -e

	Ajoute cette ligne à la fin :

		@reboot /usr/local/bin/socat_usb0.sh

	Puis redémarre pour tester.
 
### SUR MON PC DANS UN TERMINAL
	sudo socat -d -d tcp:192.168.2.2:22222 PTY,raw,echo=0,link=/dev/ttyVUSB0,mode=666



