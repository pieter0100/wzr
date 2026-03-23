#include <windows.h>
#include <math.h>
#include <time.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <iterator> 
#include <map>
#include <iostream>

#include "objects.h"
#include "graphics.h"
#include "net.h"
using namespace std;

struct Frame                                      // g��wna struktura s�u��ca do przesy�ania informacji
{
	int iID;                                      // identyfikator obiektu, kt�rego 
	int type;                                     // typ ramki: informacja o stateie, informacja o zamkni�ciu, komunikat tekstowy, ... 
	ObjectState state;                            // po�o�enie, pr�dko��: �rodka masy + k�towe, ...

	long sending_time;                            // tzw. znacznik czasu potrzebny np. do obliczenia op�nienia
	int iID_receiver;                             // nr ID odbiorcy wiadomo�ci, je�li skierowana jest tylko do niego
};

unicast_net* uni_reciv;
unicast_net* uni_send;

int main() {
	uni_reciv = new unicast_net(1002);
	uni_send = new unicast_net(1001);

	map<unsigned long, time_t> clients;
	Frame frame;
	unsigned long sender_ip;

	cout << "active server" << endl;

	while (1) {
		if (uni_reciv->reciv((char*)&frame, &sender_ip, sizeof(Frame)) > 0) {
			clients[sender_ip] = time(NULL);
		}

		for (map<unsigned long, time_t>::iterator it = clients.begin(); it != clients.end(); ++it) {
			unsigned long ip_current_client = it->first;
			if (ip_current_client != sender_ip) {
				uni_send->send((char*)&frame, ip_current_client, sizeof(Frame));
				cout << "frame from " << sender_ip << " to " << ip_current_client << endl;
			}
		}
	}
}