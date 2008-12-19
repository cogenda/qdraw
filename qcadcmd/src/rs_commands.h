/****************************************************************************
** $Id: rs_commands.h 1307 2004-04-04 22:48:13Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/


#ifndef RS_COMMANDS_H
#define RS_COMMANDS_H

#include <qobject.h>

#include "rs.h"
#include "rs_dict.h"
#include "rs_string.h"
#include "rs_stringlist.h"

#define RS_COMMANDS RS_Commands::instance()

/**
 * Class which holds all commands for the command line. This 
 * is separated in this module to alow the use of different
 * languages for the gui and the command interface.
 * Implemented as singleton. 
 *
 * @author Andrew Mustun
 */
class RS_Commands : public QObject {
    Q_OBJECT

public:
    /**
     * @return Instance to the unique commands object.
     */
    static RS_Commands* instance() {
        if (uniqueInstance==NULL) {
            uniqueInstance = new RS_Commands();
        }
        return uniqueInstance;
    }

	RS_StringList complete(const RS_String& cmd);
    RS2::ActionType cmdToAction(const RS_String& cmd);
    RS2::ActionType keycodeToAction(const RS_String& code);

    static RS_String command(const RS_String& cmd);

    static bool checkCommand(const RS_String& cmd, const RS_String& str,
                             RS2::ActionType action=RS2::ActionNone);

	static RS_String msgAvailableCommands();

protected:
    static RS_Commands* uniqueInstance;

private:
	RS_Commands();
	RS_Dict<RS2::ActionType> mainCommands;
	RS_Dict<RS2::ActionType> shortCommands;
};

#endif

