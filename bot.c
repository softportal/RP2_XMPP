/* bot.c
** libstrophe XMPP client library -- basic usage example
**
** Copyright (C) 2005-2009 Collecta, Inc. 
**
**  This software is provided AS-IS with no warranty, either express
**  or implied.
**
** This program is dual licensed under the MIT and GPLv3 licenses.
*/

/* simple bot example
**  
** This example was provided by Matthew Wild <mwild1@gmail.com>.
**
** This bot responds to basic messages and iq version requests.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <strophe.h>

#define TEMPFILE	((const unsigned char *)"/tmp/temp")


int version_handler(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
    xmpp_stanza_t *reply, *query, *name, *version, *text;
    const char *ns;
    xmpp_ctx_t *ctx = (xmpp_ctx_t*)userdata;

    printf("Received version request from %s\n", xmpp_stanza_get_from(stanza));

    reply = xmpp_stanza_reply(stanza);
    xmpp_stanza_set_type(reply, "result");

    query = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(query, "query");
    ns = xmpp_stanza_get_ns(xmpp_stanza_get_children(stanza));
    if (ns) {
        xmpp_stanza_set_ns(query, ns);
    }

    name = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(name, "name");
    xmpp_stanza_add_child(query, name);
    xmpp_stanza_release(name);

    text = xmpp_stanza_new(ctx);
    xmpp_stanza_set_text(text, "libstrophe example bot");
    xmpp_stanza_add_child(name, text);
    xmpp_stanza_release(text);

    version = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(version, "version");
    xmpp_stanza_add_child(query, version);
    xmpp_stanza_release(version);

    text = xmpp_stanza_new(ctx);
    xmpp_stanza_set_text(text, "1.0");
    xmpp_stanza_add_child(version, text);
    xmpp_stanza_release(text);

    xmpp_stanza_add_child(reply, query);
    xmpp_stanza_release(query);

    xmpp_send(conn, reply);
    xmpp_stanza_release(reply);
    return 1;
}


int message_handler(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
    xmpp_ctx_t *ctx = (xmpp_ctx_t*)userdata;
    xmpp_stanza_t *body, *reply;
    const char *type;
    char *intext, *replytext;
	FILE *sync;
    int quit = 0;
	char line[128]={0,}; // Initialize memory! You have to do this (as for your question)
    char buf_temp[128];

    body = xmpp_stanza_get_child_by_name(stanza, "body");
    if (body == NULL)
        return 1;
    type = xmpp_stanza_get_type(stanza);
    if (type != NULL && strcmp(type, "error") == 0)
        return 1;

    intext = xmpp_stanza_get_text(body);

    printf("Incoming message from %s: %s\n", xmpp_stanza_get_from(stanza), intext);

    reply = xmpp_stanza_reply(stanza);
    if (xmpp_stanza_get_type(reply) == NULL)
        xmpp_stanza_set_type(reply, "chat");

    if (strcmp(intext, "quit") == 0) {
        replytext = strdup("bye!");
        quit = 1;
    } else {

	    sync = fopen("/tmp/temp", "r");
	    if( sync ) {
	       while( fgets(line, 128, sync) !=NULL ) {}
	       printf("Last line %s\n", line); //<this is just a log... you can remove it
	       fclose(sync);
	    }

		sprintf(buf_temp, " %s",line);
        replytext = (char *) malloc(128 + strlen(intext) + 1);
        strcpy(replytext, intext);
        strcat(replytext, buf_temp);
    }
    xmpp_free(ctx, intext);
    xmpp_message_set_body(reply, replytext);

    xmpp_send(conn, reply);
    xmpp_stanza_release(reply);
    free(replytext);

    if (quit)
        xmpp_disconnect(conn);

    return 1;
}

/* define a handler for connection events */
void conn_handler(xmpp_conn_t * const conn, const xmpp_conn_event_t status, 
                  const int error, xmpp_stream_error_t * const stream_error,
                  void * const userdata)
{
    xmpp_ctx_t *ctx = (xmpp_ctx_t *)userdata;

    if (status == XMPP_CONN_CONNECT) {
        xmpp_stanza_t* pres;
        fprintf(stderr, "DEBUG: connected\n");
        xmpp_handler_add(conn, version_handler, "jabber:iq:version", "iq", NULL, ctx);
        xmpp_handler_add(conn, message_handler, NULL, "message", NULL, ctx);

        /* Send initial <presence/> so that we appear online to contacts */
        pres = xmpp_presence_new(ctx);
        xmpp_send(conn, pres);
        xmpp_stanza_release(pres);
    }
    else {
        fprintf(stderr, "DEBUG: disconnected\n");
        xmpp_stop(ctx);
    }
}

int main(int argc, char **argv)
{
    xmpp_ctx_t *ctx;
    xmpp_conn_t *conn;
    xmpp_log_t *log;
    char *jid, *pass;

    //**********************************************************************
    // CLI PARSING
    //************************************************************************
    /* take a jid and password on the command line */
    if (argc != 3) {
        fprintf(stderr, "Usage: bot <jid> <pass>\n\n");
        return 1;
    }

    //******************************************************************


    pid_t pID = fork();
	if (pID == 0)                // child
    {
        int fd = open("/tmp/temp", O_WRONLY|O_CREAT, 0666);
        int cli = dup2(fd, 1);
        close(fd);

        execlp("mosquitto_sub","mosquitto_sub","-h", "192.168.1.91","-p", "1883", "-t", "topic1",(char*) NULL);
    }
    else if (pID < 0)
    {
        printf("fork error!");
        exit(1);
    }
    else
    {           //main

        jid = argv[1];
        pass = argv[2];

        /* init library */
        xmpp_initialize();

        /* create a context */
        log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG); /* pass NULL instead to silence output */
        ctx = xmpp_ctx_new(NULL, log);

        /* create a connection */
        conn = xmpp_conn_new(ctx);

        /*
         * also you can disable TLS support or force legacy SSL
         * connection without STARTTLS
         *
         * see xmpp_conn_set_flags() or examples/basic.c
         */

        /* setup authentication information */
        xmpp_conn_set_jid(conn, jid);
        xmpp_conn_set_pass(conn, pass);

        /* initiate connection */
        xmpp_connect_client(conn, NULL, 0, conn_handler, ctx);

        /* enter the event loop - 
           our connect handler will trigger an exit */
        xmpp_run(ctx);

        /* release our connection and context */
        xmpp_conn_release(conn);
        xmpp_ctx_free(ctx);

        /* final shutdown of the library */
        xmpp_shutdown();

        system("kill -9 $(ps -ef | grep mosquitto_sub | head -n 1 | awk '{print $2}')");
    }

    return 0;
}
