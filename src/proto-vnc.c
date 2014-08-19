#include "proto-vnc.h"
#include "proto-banner1.h"
#include "unusedparm.h"
#include "masscan-app.h"
#include <ctype.h>


/***************************************************************************
 ***************************************************************************/
static void
vnc_parse(  const struct Banner1 *banner1,
          void *banner1_private,
          struct ProtocolState *pstate,
          const unsigned char *px, size_t length,
          struct BannerOutput *banout,
          struct InteractiveData *more)
{
    unsigned state = pstate->state;
    unsigned i;
    
    UNUSEDPARM(banner1_private);
    UNUSEDPARM(banner1);
    UNUSEDPARM(more);
    
    for (i=0; i<length; i++)
        switch (state) {
            case 0:
                if (px[i] == '\r')
                    continue;
                if (px[i] == '\n' || px[i] == '\0' || !isprint(px[i])) {
                    state = STATE_DONE;
                    continue;
                }
                banout_append_char(banout, PROTO_VNC_RFB, px[i]);
                break;
            default:
                i = (unsigned)length;
                break;
        }
    pstate->state = state;
}

/***************************************************************************
 ***************************************************************************/
static void *
vnc_init(struct Banner1 *banner1)
{
    UNUSEDPARM(banner1);
    return 0;
}


/***************************************************************************
 ***************************************************************************/
static int
vnc_selftest(void)
{
    return 0;
}

/***************************************************************************
 ***************************************************************************/
const struct ProtocolParserStream banner_vnc = {
    "vnc", 5900, 0, 0, 0,
    vnc_selftest,
    vnc_init,
    vnc_parse,
};
