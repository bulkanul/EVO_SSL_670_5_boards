#ifndef SERVER_DEF
#define SERVER_DEF

#include "global.h"
                        
#ifndef TB_DEF
#include "lwip/tcp.h"
#include "api.h"
#include "ethernetif.h"
#include "stdio.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#endif

void server_handler(QueueHandle_t* tcp_rx_data_queue,QueueHandle_t* indication_queue);
struct netconn* getRemoteClient();
void ethernetif_notify_conn_changed(struct netif *netif);

#endif //SERVER_DEF
