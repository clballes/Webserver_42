/*                                                                            */
/* HTTP_status_codes.hpp                                                      */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Fri Mar  8 16:20:58 2024                                                   */

#pragma once

#define CONTINUE				100
#define SWITCHING_PROTOCOLS		101
#define PROCESSING				102
#define EARLY_HINTS				103

#define OK						200
#define CREATED					201
#define ACCEPTED				202
#define NON_AUTH_INFO			203
#define NO_CONTENT				204
#define RESET_CONTENT			205
#define PARTIAL_CONTENT			206
#define MULTI_STATUS			207
#define ALREADY_REPORTED		208

#define MULTIPLE_CHOICES		300
#define MOVED_PERMANENTLY		301
#define FOUND					302
#define SEE_OTHER				303
#define NOT_MODIFIED			304
#define TEMP_REDIRECT			307
#define PERMANENT_REDIRECT		308

#define BAD_REQUEST				400
#define UNAUTHORIZED			401
#define PAYMENT_REQUIERED		402
#define FORBIDDEN				403
#define NOT_FOUND				404
#define METHOD_NOT_ALLOWED		405
#define NOT_ACCEPTABLE			406
#define PROXY_AUTH_REQUIRED		407
#define REQUEST_TIMEOUT			408
#define CONFLICT				409
#define GONE					410
#define LENGTH_REQUIRED			411
#define PRECONDITION_FAILED		412
#define CONTENT_TOO_LARGE		413
#define URI_TOO_LONG			414
#define UNSUPPORTED_MEDIA_TYPE	415
#define RANGE_NOT_SATISFIABLE	416
#define EXPECTATION_FAILED		417
#define IM_A_TEAPOT				418
#define MISDIRECTED_REQUEST		421
#define UNPROCESSABLE_CONTENT	422
#define LOCKED					423
#define FAILED_DEPENDENCY		424
#define TOO_EARLY				425
#define UPGRADE_REQUIRED		426
#define TOO_MANY_REQUESTS		429
#define HEADER_FIELDS_TOO_LARGE	431

#define INTERNAL_SERVER_ERROR	500
#define NOT_IMPLEMENTED			501
#define BAD_GATEWAY				502
#define SERVICE_UNAVAILABLE		503
#define GATEWAY_TIMEOUT			504
#define HTTP_VER_NOT_SUPPORTED	505
#define INSUFFICIENT_STORAGE 	507
#define LOOP_DETECTED 			508
#define NOT_EXTENDED 			510
#define NETWORK_AUTH_REQUIRED	511
