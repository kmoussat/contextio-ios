//
//  CIOMessageRequests.h
//  CIOAPIClient
//
//  Created by Alex Pretzlav on 7/23/15.
//  Copyright (c) 2015 Context.io. All rights reserved.
//

#import "CIOSearchRequest.h"
#import "CIOMessageFlags.h"

@class CIOAPIClient;


/**
 *  A request for a list of messages for an account. See https://context.io/docs/2.0/accounts/messages
 
 Each of the `email`, `to`, `from`, `cc` and `bcc` parameters can be set to a comma-separated list of email addresses. These multiple addresses are treated as an OR combination.

 You can set more than one parameter when doing this call. Multiple parameters are treated as an `AND` combination.
 */
@interface CIOMessagesRequest : CIOSearchRequest

/**
 *  Get messages whose subject matches this search string. To use regular expressions instead of simple string matching, make sure the string starts and ends with `/`.
 */
@property (nullable, nonatomic) NSString *subject;

/**
 *  Filter messages by the folder (or Gmail label). This parameter can be the complete folder name with the appropriate hierarchy delimiter for the mail server being queried (eg. `Inbox/My folder`) or the "symbolic name" of the folder (eg. `\Starred`). The symbolic name refers to attributes used to refer to special use folders in a language-independent way. See [RFC-6154](http://tools.ietf.org/html/rfc6154).
 */
@property (nullable, nonatomic) NSString *folder;

/**
 *  Set to `YES` to include thread size in the result.
 */
@property (nonatomic) BOOL include_thread_size;
/**
 *  Set to `YES` to include message bodies in the result. Since message bodies must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 */
@property (nonatomic) BOOL include_body;
/**
 *  Can be set to `0` (default), `1` or `raw`. If set to `1`, complete message headers, parsed into an array, are included in the results. If set to `raw`, the headers are also included but as a raw unparsed string. Since full original headers bodies must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 */
@property (nullable, nonatomic) NSString *include_headers;
/**
 *  Set to `YES` to include IMAP flags of messages in the result. Since message flags must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 */
@property (nonatomic) BOOL include_flags;
/**
 *  Used when `include_body` is set to get only body parts of a given MIME-type (for example `text/html`)
 */
@property (nullable, nonatomic) NSString *body_type;
/**
 *  Set to `YES` to include message sources in the result. Since message sources must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 */
@property (nonatomic) BOOL include_source;

/**
 *  A request to fetch messages for a given account.
 *
 *  @param accountID The account ID to fetch messages for
 *  @param client    optional CIOAPIClient to execute this request
 *
 *  @return a new CIOMessagesRequest
 */
+ (nonnull instancetype)requestForAccountId:(nonnull NSString *)accountID client:(nullable CIOAPIClient *)client;

@end

/**
 Encapsulates the parameters allowed in https://context.io/docs/2.0/accounts/messages/thread
 */
@interface CIOThreadRequest : CIODictionaryRequest

/**
 *  Set to `YES` to include message bodies in the result. Since message bodies must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 */
@property (nonatomic) BOOL include_body;

/**
 *  Can be set to `0` (default), `1` or `raw`. If set to `1`, complete message headers, parsed into an array, are included in the results. If set to `raw`, the headers are also included but as a raw unparsed string. Since full original headers bodies must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 */
@property (nullable, nonatomic) NSString *include_headers;

/**
 *  Set to `YES` to include IMAP flags of messages in the result. Since message flags must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 */
@property (nonatomic) BOOL include_flags;

/**
 *  Used when `include_body` is set to get only body parts of a given MIME-type (for example `text/html`)
 */
@property (nullable, nonatomic) NSString *body_type;

/**
 The maximum number of messages to include in the messages property of the response. The maximum limit is `100`.
 */
@property (nonatomic) NSInteger limit;

/**
 	Start the list of messages at this offset (zero-based).
 */
@property (nonatomic) NSInteger offset;


@end

/**
 A request for a single message in an account. The `limit` and `offset` parameters are ignored.
 */
@interface CIOMessageRequest : CIOThreadRequest

/**
 *  Set to `YES` to include thread size in the result.
 */
@property (nonatomic) BOOL include_thread_size;

/**
 *  Set to `YES` to include message sources in the result. Since message sources must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 */
@property (nonatomic) BOOL include_source;

@end

/**
 A request to update a single message

 @see https://context.io/docs/2.0/accounts/messages#id-post
 */
@interface CIOMessageUpdateRequest : CIODictionaryRequest

/**
 Label of the `source` you want the message copied to. This field is required if you're moving a message that already exists in one source of the account to another source of that account. If you only want to move the message to a different folder within the same source, `dst_folder` is sufficient.
 */
@property (nullable, nonatomic) NSString *dst_source;

/**
 By default, this calls copies the original message in the destination. Set this parameter to `YES` to move instead of copy.
 */
@property (nonatomic) BOOL move;

/**
 Flags to add or remove on the message being updated.
 */
@property (nonnull, nonatomic) CIOMessageFlags *flags;

@end

@interface CIOFolderMessagesRequest : CIOArrayRequest

/**
 *  Set to `YES` to include thread size in the result.
 */
@property (nonatomic) BOOL include_thread_size;

/**
 *  Set to `YES` to include message bodies in the result. Since message bodies must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 */
@property (nonatomic) BOOL include_body;

/**
 *  Used when `include_body` is set to get only body parts of a given MIME-type (for example `text/html`)
 */
@property (nullable, nonatomic) NSString *body_type;

/**
 *  Can be set to `0` (default), `1` or `raw`. If set to `1`, complete message headers, parsed into an array, are included in the results. If set to `raw`, the headers are also included but as a raw unparsed string. Since full original headers bodies must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 */
@property (nullable, nonatomic) NSString *include_headers;

/**
 *  Set to `YES` to include IMAP flags of messages in the result. Since message flags must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 */
@property (nonatomic) BOOL include_flags;

/**
 Set to `@YES` to restrict list to messages having the `\Seen` flag set, set to `@NO` to have the messages with that flag unset (ie. list unread messages in the folder).
 */
@property (nullable, nonatomic) NSNumber *flag_seen;

/**
 Set to `YES` to run job asynchronously.
 */
@property (nonatomic) BOOL async;

@end