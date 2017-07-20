//
//  so.h
//  node-pdf
//
//  Created by Pedro Lucas on 20/7/16.
//  Copyright © 2017 Appmania. All rights reserved.
//

#ifndef OS_H
#define OS_H

#if !defined(IS_MACOSX) && defined(__APPLE__) && defined(__MACH__)
	#define IS_MACOSX
#endif /* IS_MACOSX */

#if !defined(IS_WINDOWS) && (defined(WIN32) || defined(_WIN32) || \
                             defined(__WIN32__) || defined(__WINDOWS__))
	#define IS_WINDOWS
#endif /* IS_WINDOWS */


#endif /* OS_H */
