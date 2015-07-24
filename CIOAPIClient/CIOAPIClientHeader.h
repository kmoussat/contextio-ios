//
//  CIOAPIClientHeader.h
//
//
//  Created by Kevin Lord on 1/10/13.
//
//

#import <Foundation/Foundation.h>
#import "CIORequest.h"
#import "CIOMessageRequests.h"
#import "CIOContactsRequest.h"
#import "CIOFilesRequest.h"
#import "CIOSourceRequests.h"

typedef NS_ENUM(NSInteger, CIOEmailProviderType) {
    CIOEmailProviderTypeGenericIMAP = 0,
    CIOEmailProviderTypeGmail = 1,
    CIOEmailProviderTypeYahoo = 2,
    CIOEmailProviderTypeAOL = 3,
    CIOEmailProviderTypeHotmail = 4,
};

NS_ASSUME_NONNULL_BEGIN

extern NSString *const CIOAPIBaseURLString;

/**
 `CIOAPIClient` provides an easy to use interface for constructing requests against the Context.IO API. The client
 handles authentication and all signing of requests.

 Each `CIOAPIClient` instance handles its own authentication credentials. If the credentials are saved to the keychain
 via `completeLoginWithResponse:saveCredentials:`, they are keyed off of the consumer key. `CIOAPIClient` will restore
 saved credentials if it is initalized with a previously-authenticated consumer key/secret.
 */
@interface CIOAPIClient : NSObject

@property (readonly, nonatomic, nullable) NSString *accountID;

/**
 The current authorization status of the API client.
 */
@property (nonatomic, readonly) BOOL isAuthorized;

/**
 The timeout interval for all requests made. Defaults to 60 seconds.
 */
@property (nonatomic) NSTimeInterval timeoutInterval;

#pragma mark - Creating and Initializing API Clients

/**
 Initializes a `CIOAPIClient` object with the specified consumer key and secret. If a previously-authenticated consumer
 key is provided, its authentcation information will be restored from the keychain.

 @param consumerKey The consumer key for the API client. This argument must not be `nil`.
 @param consumerSecret The consumer secret for the API client. This argument must not be `nil`.

 @return The newly-initialized API client
 */
- (instancetype)initWithConsumerKey:(NSString *)consumerKey consumerSecret:(NSString *)consumerSecret;

/**
 Initializes a `CIOAPIClient` object with the specified consumer key and secret, and additionally token and token
 secret. Use this method if you have already obtained a token and token secret on your own, and do not wish to use the
 built-in keychain storage.

 @param consumerKey The consumer key for the API client. This argument must not be `nil`.
 @param consumerSecret The consumer secret for the API client. This argument must not be `nil`.
 @param token The auth token for the API client.
 @param tokenSecret The auth token secret for the API client.
 @param accountID The account ID the client should use to construct requests.

 @return The newly-initialized API client
 */
- (instancetype)initWithConsumerKey:(NSString *)consumerKey
                     consumerSecret:(NSString *)consumerSecret
                              token:(nullable NSString *)token
                        tokenSecret:(nullable NSString *)tokenSecret
                          accountID:(nullable NSString *)accountID NS_DESIGNATED_INITIALIZER;

/**
 *  Create a signed `NSURLRequest` for the context.io API using current OAuth credentials
 *
 *  @param path   path in the 2.0 API namespace, e.g. "accounts/<id>/contacts"
 *  @param method HTTP request method
 *  @param params parameters to send, will be sent as URL params for GET, otherwise sent as a `x-www-form-urlencoded`
 * body
 *
 */
- (NSURLRequest *)requestForPath:(NSString *)path method:(NSString *)method params:(nullable NSDictionary *)params;

- (NSURLRequest *)requestForCIORequest:(CIORequest *)request;

#pragma mark - Authenticating the API Client

/**
 Begins the authentication process for a new account/email source by creating a connect token.

 @param providerType The type of email provider you would like to authenticate. Please see `CIOEmailProviderType`.
 @param callbackURLString The callback URL string that the API should redirect to after successful authentication of an
 email account. You will need to watch for this request in your UIWebView delegate's
 -webView:shouldStartLoadWithRequest:navigationType: method to intercept the connect token. See the example app for
 details.
 @param params The parameters for the request. This can be `nil` if no parameters are required.
 */
- (CIODictionaryRequest *)beginAuthForProviderType:(CIOEmailProviderType)providerType
                                 callbackURLString:(NSString *)callbackURLString
                                            params:(nullable NSDictionary *)params;

- (NSURL *)redirectURLFromResponse:(NSDictionary *)responseDict;

- (CIODictionaryRequest *)fetchAccountWithConnectToken:(NSString *)connectToken;

/**
 Uses the connect token received from the API to complete the authentication process and optionally save the credentials
 to the keychain.

 @param connectToken The connect token returned by the API after the user successfully authenticates an email account.
 This is returned as a query parameter appended to the callback URL that the API uses as a final redirect.
 @param saveCredentials This determines if credentials are saved to the device's keychain.
 */
- (BOOL)completeLoginWithResponse:(NSDictionary *)responseObject saveCredentials:(BOOL)saveCredentials;

/**
 Clears the credentials stored in the keychain.
 */
- (void)clearCredentials;

#pragma mark - Working With Accounts

/**
 *   Retrieves an account's details.
 *
 *  @param accountId Unique id of an account accessible through your API key
 *
 */
- (CIODictionaryRequest *)getAccount;

/**
 *  Modify an account's info.
 *
 *  @param accountId Unique id of an account accessible through your API key
 *  @param firstName new first name, optional
 *  @param lastName  new last name, optional
 *
 *  @return request to modify the account
 */
- (CIODictionaryRequest *)updateAccountWithFirstName:(nullable NSString *)firstName lastName:(nullable NSString *)lastName;


/**
 *  Deletes an account with the specified ID
 *
 *  @param identifier Unique id of an account accessible through your API key
 *
 */
- (CIODictionaryRequest *)deleteAccount;

#pragma mark - Working With Contacts and Related Resources

/**
 Retrieves the account's contacts.

 @param params A dictionary of parameters to be sent with the request. See the API documentation for possible
 parameters.
 */
- (CIOContactsRequest *)getContacts;

/**
 Retrieves the contact with the specified email.

 @param email The email address of the contact you would like to retrieve.
 */
- (CIODictionaryRequest *)getContactWithEmail:(NSString *)email;

/**
 List files exchanged with a contact.

 This call returns the latest attachments exchanged with one or more email addresses. By "exchanged with Mr. X" we mean any file attached to an email received from Mr. X, sent to Mr. X or sent by anyone to both Mr. X and the mailbox owner.

 @see You can get list of messages with more precise filtering using a combination of the to, from and cc filters by using `getFiles`.

 @param email The email address of the contact for which you would like to retrieve associated messages.
 */
- (CIOArrayRequest *)getFilesForContactWithEmail:(NSString *)email;

/**
 Listing messages where a contact is present

 This call returns the latest email messages exchanged with one or more email addresses. By "exchanged with Mr. X" we mean any email received from Mr. X, sent to Mr. X or sent by anyone to both Mr. X and the mailbox owner.
 
 @see You can get list of messages with more precise filtering using a combination of the to, from and cc filters using `getMessages`.

 @param email Email address of a contact
 */
- (CIOArrayRequest *)getMessagesForContactWithEmail:(NSString *)email;

/**
 Listing threads where a contact is present

 This call returns the latest email threads exchanged with one or more email addresses. By "exchanged with Mr. X" we mean any email received from Mr. X, sent to Mr. X or sent by anyone to both Mr. X and the mailbox owner.
 
 @see `getThreads`, you can get list of messages with more precise filtering using a combination of the to, from and cc filters

 @param email Email address of a contact
 */
- (CIOArrayRequest *)getThreadsForContactWithEmail:(NSString *)email;

#pragma mark - Working With Email Address Aliases

/**
 List of email addressed used by an account

 List your configured user account emails.
 */
- (CIOArrayRequest *)getEmailAddresses;

/**
 Add a new email addresses as an alias for an account

 @param email The email address you would like to associate with the account.
 */
- (CIODictionaryRequest *)addEmailAddress:(NSString *)email;

/**
 Updates the details of a particular email address.

 @param email The email address for which you would like to update details.
 @param primary Set to `YES` to make this email address the primary one for the account
 */
- (CIODictionaryRequest *)updateEmailAddressWithEmail:(NSString *)email primary:(BOOL)primary;

/**
 Disassociates a particular email address from the account.

 @param email The email address you would like to disassociate from the account.
 */
- (CIODictionaryRequest *)deleteEmailAddressWithEmail:(NSString *)email;

#pragma mark - Working With Files and Related Resources

/**
 
 List details of files found as email attachments.

 @see `CIOFilesRequest` and `CIOSearchRequest` for a description of the possible parameters you can pass in this method.
 
 @see https://context.io/docs/2.0/accounts/files
 */
- (CIOFilesRequest *)getFiles;

/**
 Retrieves details about the file with the specified id.

 @param fileID The id of the file you would like to retrieve.
 */
- (CIODictionaryRequest *)getDetailsOfFileWithID:(NSString *)fileID;

/**
 List of files that can be compared with a given file

 @param fileID The id of the file for which you would like to retrieve changes.
 */
- (CIOArrayRequest *)getChangesForFileWithID:(NSString *)fileID;

/**
 Retrieves a public facing URL that can be used to download a particular file.

 @param fileID The id of the file that you would like to download.
 @param params A dictionary of parameters to be sent with the request. See the API documentation for possible
 parameters.
 */
- (CIOStringRequest *)getContentsURLForFileWithID:(NSString *)fileID;

/**
 Retrieves the contents of a particular file.

 @param fileID The id of the file that you would like to download.
 @param saveToPath The local file path where you would like to save the contents of the file.
 */
- (CIORequest *)downloadContentsOfFileWithID:(NSString *)fileID;

/**
 Retrieves other files associated with a particular file.
 
 Returns a list of files that are related to the given file. Currently, relation between files is based on how similar their names are.

 @param fileID The id of the file for which you would like to retrieve associated files.
 */
- (CIOArrayRequest *)getRelatedForFileWithID:(NSString *)fileID;

/**
 Retrieves the revisions of a particular file.
 
 Returns a list of revisions attached to other emails in the mailbox for a given file.

 Two files are considered revisions of the same document if their file names are identical outside of revision indicators such as dates, author initials, version numbers and keywords like "final" or "draft".

 @param fileID The id of the file for which you would like to retrieve revisions.
 */
- (CIOArrayRequest *)getRevisionsForFileWithID:(NSString *)fileID;

#pragma mark - Working With Messages and Related Resources

/**
 List email messages for an account.
 
 https://context.io/docs/2.0/accounts/messages
 
 @see `CIOMessagesRequest` and `CIOSearchRequest` for possible parameters to use in this call.
 */
- (CIOMessagesRequest *)getMessages;

/**
 Get file, contact and other information about a given email message.
 
 @see `CIOMessageRequest` for optional parameters you can set on this request.

 @param messageID Unique id of a message. This can be the message_id or email_message_id property of the message. The gmail_message_id (prefixed with gm-) can also be used.
 */
- (CIOMessageRequest *)getMessageWithID:(NSString *)messageID;

/**
 Copy or move a message

 This call allows you to copy or move a message between folders. If there are more than one sources on the account, you can use this call to copy/move the message between these sources. In this case, the `dst_label` parameter must identify the source you want to message to be copied/moved to. See `CIOMessageUpdateRequest` for further optional parameters.

 Have a look at `getFoldersForMessageWithID:` to get a more flexible way to manage which folders a message should appear in.
 
 @see `getFoldersForMessageWithID:`
 @see `CIOMessageUpdateRequest`

 @param messageID Unique id of a message. This can be the message_id or email_message_id property of the message. The gmail_message_id (prefixed with gm-) can also be used.

 @param destinationFolder The folder within `dst_source` the message should be copied to
 */
- (CIOMessageUpdateRequest *)updateMessageWithID:(NSString *)messageID
                            destinationFolder:(NSString *)destinationFolder;

/**
 Deletes the message with the specified id.
 
 @discussion <h3>For standard IMAP accounts</h3>

 The `DELETE` method will delete messages from the source email server. In IMAP-speak, it will set the `\\Deleted` flag on the message and then call the `EXPUNGE` command.

 Once this is done, the message is gone forever, there's no way to bring it back.

 <blockquote>The `EXPUNGE` command removes all messages in that folder that have the `\\Deleted` flag set. If you want to avoid that, you can simply mark the message as deleted.</blockquote>

 <h3>For Gmail and Google Apps accounts</h3>

 For Gmail and Google Apps mailboxes, setting the `\\Deleted` flag and calling the `EXPUNGE` command is equivalent to removing a "label" and results in that message still showing up in the "All Mail" folder and other labels also assigned to the message. See this Gmail Support entry for details: http://support.google.com/mail/bin/answer.py?hl=en&answer=78755

 So, if you call the `DELETE` method on a message we assume it's because you wanted that message deleted. Therefore, what actually happens for Gmail or Google Apps messages is we move it to the "[Gmail]/Trash" folder.

 @param messageID Unique id of a message. This can be the message_id or email_message_id property of the message. The gmail_message_id (prefixed with gm-) can also be used.
 */
- (CIODictionaryRequest *)deleteMessageWithID:(NSString *)messageID;

/**
 Fetch the message body of a given email.
 
 https://context.io/docs/2.0/accounts/messages/body
 
 <h3>Retrieving attachments</h3>

 This call only returns text portions of messages, attachments aren't included. To get a list of attachments on the message, look for the files property in the response of a message instance. To download the content of these attachments, use the files/content call
 
 <h3>On-demand data retrieval</h3>

 Since we do not keep full copies of emails on our servers, this call triggers a connection to the IMAP server to fetch the message. One thing to point out is we do fetch messages in such a way that large files attached to a message won't make any difference in the response time.
 
 @see `getMessageWithID:` If you want to fetch headers, body and flags for a given message, you can use the related include_* GET parameters on `CIOMessageRequest` to get them all in a single call.

 @param messageID Unique id of a message. This can be the message_id or email_message_id property of the message. The gmail_message_id (prefixed with gm-) can also be used.
 @param type Many emails are sent with both rich text and plain text versions in the message body and by default, the response of this call will include both. It is possible to only get either the plain or rich text version by setting the `type` parameter to `text/plain` or `text/html` respectively.
 */
- (CIOArrayRequest *)getBodyForMessageWithID:(NSString *)messageID type:(nullable NSString *)type;

/**
 Retrieves the flags for a particular message.
 
 The Flags resource allows you set to retrieve and set flags that indicate, among others, if the message has been read.

 You can find detailed explanation about each IMAP message flags here: http://tools.ietf.org/html/rfc3501#section-2.3.2.
 
 https://context.io/docs/2.0/accounts/messages/flags
 
 <h3>On-demand data retrieval</h3>

 To ensure up-to-date values, flags are not cached by Context.IO. This call triggers a connection to the IMAP server to fetch the current message flags before it returns.

 @param messageID Unique id of a message. This can be the message_id or email_message_id property of the message. The gmail_message_id (prefixed with gm-) can also be used.
 */
- (CIODictionaryRequest *)getFlagsForMessageWithID:(NSString *)messageID;

/**
 Updates the flags for a particular message.

 @param messageID The id of the message for which you would like to update the flags.
 @param flags A `CIOMessageFlags` object which defines which flags to add or remove from the message
 */
- (CIODictionaryRequest *)updateFlagsForMessageWithID:(NSString *)messageID flags:(CIOMessageFlags *)flags;
/**
 Retrieves the folders for a particular message.

 Note that for Gmail and Google Apps, this resource maps to the "labels" assigned to the message.

 https://context.io/docs/2.0/accounts/messages/folders

 @param messageID The id of the message for which you would like to retrieve the folders.
 */
- (CIOArrayRequest *)getFoldersForMessageWithID:(NSString *)messageID;

/**
 Updates the folders for a particular message.

 While you can copy/move a message in a given folder with the POST method on a given message, this allows a more flexible way to add or remove folder(s) a message should appear in.
 
 @note This SDK does not currently support adding or removing more than one folder in a single API call.

 @param messageID Unique id of a message. This can be the message_id or email_message_id property of the message. The gmail_message_id (prefixed with gm-) can also be used.
 */
- (CIODictionaryRequest *)updateFoldersForMessageWithID:(NSString *)messageID addToFolder:(nullable NSString *)addFolder removeFromFolder:(nullable NSString *)removeFolder;

/**
 Set folders a message should appear
 
 TODO: This is currently getting OAuth signing errors. Needs more investigation.

 @param messageID Unique id of a message. This can be the message_id or email_message_id property of the message. The gmail_message_id (prefixed with gm-) can also be used.
 @param folderNames         Array of IMAP folder names
 @param symbolicFolderNames Array of 'Special-use attribute of a folder (if and only if the server supports it and applicable to this folder)'
 */
- (CIODictionaryRequest *)setFoldersForMessageWithID:(NSString *)messageID folderNames:(NSArray *)folderNames symbolicFolderNames:(NSArray *)symbolicFolderNames;

/**
 Complete headers of a given email message

 https://context.io/docs/2.0/accounts/messages/headers

 Note that you only need to specify either the name or symbolic_name property for each folder the message must appear in.

 <h3>On-demand data retrieval</h3>

 Since we do not keep full copies of emails on our servers, this call triggers a connection to the IMAP server to fetch the message headers.

 <h3>Why are all values in arrays?</h3>

 It is very common to have more than one occurrence of some headers so for those, an array makes obvious sense, for the rest, yes, we could have left them as plain old strings. However, it would be seriously annoying to always test whether the value for a given header is an array or a string whevener you want to access it. Putting everything in strings makes it easy to uniformly access them.

 @param messageID Unique id of a message. This can be the message_id or email_message_id property of the message. The gmail_message_id (prefixed with gm-) can also be used.
 */
- (CIODictionaryRequest *)getHeadersForMessageWithID:(NSString *)messageID;

/**
 Complete headers of a given email message as a raw string

 @param messageID Unique id of a message. This can be the message_id or email_message_id property of the message. The gmail_message_id (prefixed with gm-) can also be used.
 */
- (CIOStringRequest *)getRawHeadersForMessageWithID:(NSString *)messageID;

/**
 Returns the raw RFC-822 message source for the message (including attachments) with no parsing or decoding to any portions of the message.

 @param messageID The id of the message for which you would like to retrieve the source.
 */
- (CIORequest *)getSourceForMessageWithID:(NSString *)messageID;

/**
 List other messages in the same thread as a given message
 
 This returns an array with the same structure as getting information on a single message for every message in the thread.

 @param messageID The id of the message for which you would like to retrieve the thread.
 */
- (CIOThreadRequest *)getThreadForMessageWithID:(NSString *)messageID;

#pragma mark - Working With Sources and Related Resources

/**
 List IMAP sources assigned for an account.
 
 <h3>Multiple sources on a single account</h3>

 You can have more than one source for a single account. In this case, data from all these sources are merged and considered as one data set on which the other accounts sub-resources interact.

 In other words, this is meant to handle users of your application with than one email account who always wants your application to show data from all of these accounts as if they were one.

 If you want to be able to query IMAP sources separately, create two accounts (one for each IMAP source)

 @param params A dictionary of parameters to be sent with the request. See the API documentation for possible
 parameters.
 */
- (CIOSourcesRequest *)getSources;

/**
 Creates a new source under the account. Note: It is usually preferred to use
 `-beginAuthForProviderType:callbackURLString:params:` to add a new source to the account.
 
 @see `CIOCreateSourceRequest` for futher optional parameters to this request.

 @param email The primary email address used to receive emails in this account.
 @param server Name or IP of the IMAP server, eg. imap.gmail.com
 @param username The username used to authenticate an IMAP connection. On some servers, this is the same thing as the primary email address.
 @param useSSL Set to `YES` if you want SSL encryption to be used when opening connections to the IMAP server. Any other value will be considered as "do not use SSL"
 @param port Port number to connect to on the server. Keep in mind that most IMAP servers will have one port for standard connection and another one for encrypted connection (see `use-ssl` parameter)
 @param type Currently, the only supported type is IMAP
 */
- (CIOSourceCreateRequest *)createSourceWithEmail:(NSString *)email
                                         server:(NSString *)server
                                       username:(NSString *)username
                                         useSSL:(BOOL)useSSL
                                           port:(NSInteger)port
                                           type:(NSString *)type;

/**
 Get parameters and status for an IMAP source.

 @param sourceLabel The label property of the source instance. You can use `@"0"` as an alias for the first source of an account.
 */
- (CIODictionaryRequest *)getSourceWithLabel:(NSString *)sourceLabel;

/**
 Modify a data source on a given account

 @param sourceLabel The label property of the source instance. You can use `@"0"` as an alias for the first source of an account.
 */
- (CIOSourceModifyRequest *)updateSourceWithLabel:(NSString *)sourceLabel;

/**
 Deletes the source with the specified label.

 @param sourceLabel The label property of the source instance. You can use `@"0"` as an alias for the first source of an account.
 */
- (CIODictionaryRequest *)deleteSourceWithLabel:(NSString *)sourceLabel;

/**
 Returns folders existing in a given IMAP account.

 @param sourceLabel           The label property of the source instance. You can use `@"0"` as an alias for the first source of an account.
 @param includeExtendedCounts Set to `YES` to include extended counts in the result (for now, the only extended count supported is number of unseen messages). Since counts must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 @param noCache               Set to `YES` to fetch the folder list directly from the IMAP server. Since data must be retrieved from the IMAP server, expect a performance hit when setting this parameter.

 */
- (CIOArrayRequest *)getFoldersForSourceWithLabel:(NSString *)sourceLabel
                            includeExtendedCounts:(BOOL)includeExtendedCounts
                                          noCache:(BOOL)noCache;


/**
 Returns information about a given folder. This call exposes IMAP related attributes and other information for a given folder.

 @param folderPath            The full folder path using / as the path hierarchy delimiter.
 @param sourceLabel           The label property of the source instance. You can use `@"0"` as an alias for the first source of an account.
 @param includeExtendedCounts Set to `YES` to include extended counts in the result (for now, the only extended count supported is number of unseen messages). Since counts must be retrieved from the IMAP server, expect a performance hit when setting this parameter.
 @param delim               If `/` isn't fancy enough as a hierarchy delimiter when specifying the folder you want to obtain, you're free to use what you want, just make sure you set this delim parameter to tell us what you're using.

 */
- (CIODictionaryRequest *)getFolderWithPath:(NSString *)folderPath
                                sourceLabel:(NSString *)sourceLabel
                      includeExtendedCounts:(BOOL)includeExtendedCounts
                                      delim:(nullable NSString *)delim;

/**
 Create a folder on an IMAP source. A new folder can be added to an IMAP source by PUTting the desired path under the sources/folders resource.
 
<h3>Working with server-specific hierarchy delimiters</h3>

 IMAP servers are free to use the character they want as the folder hierarchy delimiter. The bad news is they don't use the same. The good news is you don't need to know this, we take care of it.

 <h4>Then what is that <code>delim</code> parameter for?</h4>

 Good question. By default, we expect you to specify the folder hierarchy using `/` as the hierarchy delimiter. For example, to create a folder called `my folder` under the folder `base folder`, you would call:

`PUT /2.0/accounts/<id>/sources/<label>/folders/base+folder/my+folder`
 No matter what's the actual hierarchy delimiter the IMAP server expects, this call will work. However, say you need to use another character as the delimiter, here's how you'd do it:

 `PUT /2.0/accounts/<id>/sources/<label>/folders/base+folder.my+folder?delim=.`

 Both examples above are equivalent and will have the same result no matter what the IMAP server expects as the actual hierarchy delimiter.

 @param folderPath  The full folder path using / as the path hierarchy delimiter.
 @param sourceLabel The label property of the source instance. You can use `@"0"` as an alias for the first source of an account.
 @param delim       If / isn't fancy enough as a hierarchy delimiter when specifying the folder you want to create, you're free to use what you want, just make sure you set this delim parameter to tell us what you're using.
 */
- (CIODictionaryRequest *)createFolderWithPath:(NSString *)folderPath
                                   sourceLabel:(NSString *)sourceLabel
                                         delim:(nullable NSString *)delim;

/**
 Permanently removes a folder from the IMAP source

 This clears out all messages in the folder and CAN'T BE UNDONE. See IMAP specs for more information.
 
 @see http://tools.ietf.org/html/rfc3501#section-6.3.4

 @param folderPath The path of the folder you would like to delete.
 @param sourceLabel The label property of the source instance. You can use `@"0"` as an alias for the first source of an account.
 */
- (CIODictionaryRequest *)deleteFolderWithPath:(NSString *)folderPath sourceLabel:(NSString *)sourceLabel;


/**
 Runs an `EXPUNGE` command on the email server. With this call, you can explicitly tell the email server to remove messages that are flagged for deletion. This will delete them permanently.

 @param folderPath  The full folder path using `/` as the path hierarchy delimiter.
 @param sourceLabel The label property of the source instance. You can use `@"0"` as an alias for the first source of an account.
 */
- (CIODictionaryRequest *)expungeFolderWithPath:(NSString *)folderPath
                                    sourceLabel:(NSString *)sourceLabel;

/**
 This call returns the same message data as the messages resource but instead of returning whatever is available from the metadata index we maintain, it checks the IMAP server for new messages and includes thoses in the response.

 The advantage is a list of message that reflects what's on the IMAP server at the moment the call is made vs. what was there the last time we synched with it. The downside is longer response time, a restricted set of parameters to filter results, and the fact the listing only works on a per-folder basis instead of all messages in the account.
 
 @see `CIOFolderMessagesRequest` for more optional parameters with this request.
 
 @warn This call will synchronously check the IMAP server for new emails and fetch them to update the index before sending the response. Don't expect to get a response in less than 1 second.

 @param folderPath  The full folder path using `/` as the path hierarchy delimiter.
 @param sourceLabel The label property of the source instance. You can use `@"0"` as an alias for the first source of an account.
 */
- (CIOFolderMessagesRequest *)getMessagesForFolderWithPath:(NSString *)folderPath
                                               sourceLabel:(NSString *)sourceLabel;

#pragma Source Sync

/**
 Sync status of a data source. This returns timestamps for the last time the source has been synced with the origin mailbox.

 @param sourceLabel The label property of the source instance. You can use `@"0"` as an alias for the first source of an account.
 */
- (CIODictionaryRequest *)getSyncStatusForSourceWithLabel:(NSString *)sourceLabel;

/**
 Trigger a sync of a data source. This will start a sync job for the source.

 @param sourceLabel The label property of the source instance. You can use `@"0"` as an alias for the first source of an account.
 */
- (CIODictionaryRequest *)forceSyncForSourceWithLabel:(NSString *)sourceLabel;

#pragma Sync

/**
 Sync status for all sources of the account. This returns timestamps for the last time a source has been synced with the origin mailbox.
 */
- (CIODictionaryRequest *)getSyncStatusForAllSources;

/**
 Trigger a sync of all sources on the account. This will start a sync job for all sources under the account.
 */
- (CIODictionaryRequest *)forceSyncForAllSources;


#pragma mark - Working With Sources and Related Resources

/**
 Retrieves the account's threads.

 @param params A dictionary of parameters to be sent with the request. See the API documentation for possible
 parameters.
 */
- (CIOArrayRequest *)getThreadsWithParams:(nullable NSDictionary *)params;

/**
 Retrieves the thread with the specified id.

 @param threadID The id of the thread you would like to retrieve.
 @param params A dictionary of parameters to be sent with the request. See the API documentation for possible
 parameters.
 */
- (CIODictionaryRequest *)getThreadWithID:(NSString *)threadID params:(nullable NSDictionary *)params;

#pragma mark - Working With Webhooks and Related Resources

/**
 Retrieves the account's webhooks.

 @param params A dictionary of parameters to be sent with the request. See the API documentation for possible
 parameters.
 */

- (CIOArrayRequest *)getWebhooksWithParams:(nullable NSDictionary *)params;

/**
 Creates a new webhook.

 @param callbackURLString A string representing the callback URL for the new webhook.
 @param failureNotificationURLString A string representing the failure notification URL for the new webhook.
 @param params A dictionary of parameters to be sent with the request. See the API documentation for possible
 parameters.
 */
- (CIODictionaryRequest *)createWebhookWithCallbackURLString:(NSString *)callbackURLString
                                failureNotificationURLString:(NSString *)failureNotificationURLString
                                                      params:(nullable NSDictionary *)params;

/**
 Retrieves the webhook with the specified id.

 @param webhookID The id of the webhook you would like to retrieve.
 @param params A dictionary of parameters to be sent with the request. See the API documentation for possible
 parameters.
 */
- (CIODictionaryRequest *)getWebhookWithID:(NSString *)webhookID params:(nullable NSDictionary *)params;

/**
 Updates the webhook with the specified id.

 @param webhookID The id of the webhook you would like to update.
 @param params A dictionary of parameters to be sent with the request. See the API documentation for possible
 parameters.
 */
- (CIODictionaryRequest *)updateWebhookWithID:(NSString *)webhookID params:(nullable NSDictionary *)params;

/**
 Deletes the webhook with the specified id.

 @param webhookID The id of the webhook you would like to delete.
 */
- (CIODictionaryRequest *)deleteWebhookWithID:(NSString *)webhookID;

@end

NS_ASSUME_NONNULL_END