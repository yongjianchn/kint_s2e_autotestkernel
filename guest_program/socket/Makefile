all:
	for tododir in accept bind connect listen send sendto socket setsockopt \
			sendmsg-cm1 sendmsg-cm2 sendmsg-2cm sendmsg-2len sendmsg-2flags sendmsg-bug; \
		do \
		cd $$tododir; make; cd ..; \
		done

