/*****************************************************************************
* Product: PELICAN crossing example
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Feb 29, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef qpn_port_h
#define qpn_port_h

#define Q_PARAM_SIZE            4
#define QF_TIMEEVT_CTR_SIZE     4

/* maximum # active objects--must match EXACTLY the QF_active[] definition  */
#define QF_MAX_ACTIVE           2

                               /* interrupt locking policy for GNU compiler */
#define QF_INT_DISABLE()        __asm volatile ("cpsid i")
#define QF_INT_ENABLE()         __asm volatile ("cpsie i")

                                  /* interrupt locking policy for ISR level */
#define QF_ISR_NEST

#include <stdint.h>       /* GNU provides C99-standard exact-width integers */

#include "qepn.h"         /* QEP-nano platform-independent public interface */
#include "qfn.h"           /* QF-nano platform-independent public interface */

#endif                                                        /* qpn_port_h */