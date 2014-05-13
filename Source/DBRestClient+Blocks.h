//
//  DBRestClient+Blocks.h
//  DBRestClient+Blocks
//
//  Created by Will Chilcutt on 5/12/14.
//  Copyright (c) 2014 NSWill. All rights reserved.
//

#import <DropboxSDK/DropboxSDK.h>

typedef void(^kDBRestClientLoadMetaDataCompletionBlock)(NSError *error, DBRestClient *restClient, DBMetadata *metadata);
typedef void(^kDBRestClientLoadDeltaCompletionBlock)(NSError *error, DBRestClient *restClient, NSArray *entries, BOOL shouldRest, NSString *cursor, BOOL hasMore);
typedef void(^kDBRestClientLoadAccountInfoCompletionBlock)(NSError *error, DBRestClient *client, DBAccountInfo *info);
typedef void(^kDBRestClientLoadFileProgressBlock)(DBRestClient *client, NSString *destinationPath, CGFloat progress);
typedef void(^kDBRestClientLoadFileCompletionBlock)(NSError *error, DBRestClient *client, NSString *destinationPath, NSString *contentType, DBMetadata *metadata);
typedef void(^kDBRestClientLoadThumbnailCompletionBlock)(NSError *error, DBRestClient *restClient, NSString *destPath, DBMetadata *metadata);
typedef void(^kDBRestClientUploadFileToPathFromPathProgressBlock)(DBRestClient *restClient, CGFloat progress, NSString *destinationPath, NSString *sourcePath, DBMetadata *metadata);
typedef void(^kDBRestClientUploadFileToPathFromPathCompletionBlock)(NSError *error, DBRestClient *restClient, NSString *destinationPath, NSString *sourcePath, DBMetadata *metadata);
typedef void(^kDBRestClientUploadFileChunkProgressBlock)(DBRestClient *restClient, CGFloat progress, NSString *uploadID, unsigned long long offset, NSString *localPath);
typedef void(^kDBRestClientUploadFileChunkCompletionBlock)(NSError *error, DBRestClient *restClient, NSString *uploadID, unsigned long long offset, NSString *localPath, NSDate *expiresDate);
typedef void(^kDBRestClientUploadFileFromUploadIDCompletionBlock)(NSError *error, DBRestClient *restClient, NSString *destinationPath, NSString *uploadID, DBMetadata *metadata);
typedef void(^kRestClientLoadedRevisionsCompletionBlock)(NSError *error, DBRestClient *restClient, NSArray *revisions, NSString *path);
typedef void(^kDBRestClientRestoreFileCompletionBlock)(NSError *error,DBRestClient *restClient, DBMetadata *metadata);
typedef void(^kDBRestClientCreateFolderCompletionBlock)(NSError *error, DBRestClient *restClient, DBMetadata *folderMetadata);
typedef void(^kDBRestClientDeletePathCompletionBlock)(NSError *error, DBRestClient *restClient, NSString *path);
typedef void(^kDBRestClientCopyPathCompletionBlock)(NSError *error, DBRestClient *restClient, NSString *fromPath, DBMetadata *to);
typedef void(^kDBRestClientCreateCopyReferenceCompletionBlock)(NSError *error, DBRestClient *restClient, NSString *copyReference, NSString *path);
typedef void(^kDBRestClientCopyReferenceToPathCompletionBlock)(NSError *error, DBRestClient *restClient, NSString *copyReference, DBMetadata *to);
typedef void(^kDBRestClientMoveCompletionBlock)(NSError *error, DBRestClient *restClient, NSString *fromPath, DBMetadata *result);
typedef void(^kRestClientSearchCompletionBlock)(NSError *error, DBRestClient *restClient, NSArray *results, NSString *path, NSString *keyword);
typedef void(^kDBRestClientLoadSharableLinkCompletionBlock)(NSError *error, DBRestClient *restClient, NSString *link, NSString *path);
typedef void(^kDBRestClientLoadStreamableURLCompletionBlock)(NSError *error, DBRestClient *restClient, NSURL *url, NSString *path);

@interface DBRestClient (Blocks)

- (void)loadMetadata:(NSString*)path withHash:(NSString*)hash withCompletionBlock:(kDBRestClientLoadMetaDataCompletionBlock)completionBlock;
- (void)loadMetadata:(NSString*)path withCompletionBlock:(kDBRestClientLoadMetaDataCompletionBlock)completionBlock;


/* This will load the metadata of a file at a given rev */
- (void)loadMetadata:(NSString *)path atRev:(NSString *)rev withCompletionBlock:(kDBRestClientLoadMetaDataCompletionBlock)completionBlock;

#pragma mark Load delta

/* Loads a list of files (represented as DBDeltaEntry objects) that have changed since the cursor was generated */
- (void)loadDelta:(NSString *)cursor withCompletionBlock:(kDBRestClientLoadDeltaCompletionBlock)completionBlock;

#pragma mark load account info

- (void)loadAccountInfoWithCompletionBlock:(kDBRestClientLoadAccountInfoCompletionBlock)completionBlock;
#pragma mark load file

/* Loads the file contents at the given root/path and stores the result into destinationPath */
- (void)loadFile:(NSString *)path intoPath:(NSString *)destinationPath withProgressBlock:(kDBRestClientLoadFileProgressBlock)progressBlock andCompletionBlock:(kDBRestClientLoadFileCompletionBlock)completionBlock;

/* This will load a file as it existed at a given rev */
- (void)loadFile:(NSString *)path atRev:(NSString *)rev intoPath:(NSString *)destPath withProgressBlock:(kDBRestClientLoadFileProgressBlock)progressBlock andCompletionBlock:(kDBRestClientLoadFileCompletionBlock)completionBlock;

#pragma mark load thumbnail

- (void)loadThumbnail:(NSString *)path ofSize:(NSString *)size intoPath:(NSString *)destinationPath withCompletionBlock:(kDBRestClientLoadThumbnailCompletionBlock)completionBlock;

#pragma mark upload file

/* Uploads a file that will be named filename to the given path on the server. sourcePath is the
 full path of the file you want to upload. If you are modifying a file, parentRev represents the
 rev of the file before you modified it as returned from the server. If you are uploading a new
 file set parentRev to nil. */
- (void)uploadFile:(NSString *)filename toPath:(NSString *)path withParentRev:(NSString *)parentRev
          fromPath:(NSString *)sourcePath withProgressBlock:(kDBRestClientUploadFileToPathFromPathProgressBlock)progressBlock andCompletionBlock:(kDBRestClientUploadFileToPathFromPathCompletionBlock)completionBlock;

/* These calls allow you to upload files in chunks, which is better for file larger than a few megabytes.
 You can append bytes to the file using -[DBRestClient uploadFileChunk:offset:uploadId:] and then call
 -[DBRestClient uploadFile:toPath:withParentRev:fromUploadId:] to turn the bytes appended at that uploadId
 into an actual file in the user's Dropbox.
 Use a nil uploadId to start uploading a new file. */
- (void)uploadFileChunk:(NSString *)uploadId offset:(unsigned long long)offset fromPath:(NSString *)localPath withProgressBlock:(kDBRestClientUploadFileChunkProgressBlock)progressBlock andCompletionBlock:(kDBRestClientUploadFileToPathFromPathCompletionBlock)completionBlock;

- (void)uploadFile:(NSString *)filename toPath:(NSString *)parentFolder withParentRev:(NSString *)parentRev
      fromUploadId:(NSString *)uploadId withCompletionBlock:(kDBRestClientUploadFileFromUploadIDCompletionBlock)completionBlock;

#pragma mark load revisions

/* Loads a list of up to 10 DBMetadata objects representing past revisions of the file at path */
- (void)loadRevisionsForFile:(NSString *)path withCompletionBlock:(kRestClientLoadedRevisionsCompletionBlock)completionnBlock;

/* Same as above but with a configurable limit to number of DBMetadata objects returned, up to 1000 */
- (void)loadRevisionsForFile:(NSString *)path limit:(NSInteger)limit withCompletionBlock:(kRestClientLoadedRevisionsCompletionBlock)completionnBlock;

#pragma mark restore file

/* Restores a file at path as it existed at the given rev and returns the metadata of the restored
 file after restoration */
- (void)restoreFile:(NSString *)path toRev:(NSString *)rev withCompletionBlock:(kDBRestClientRestoreFileCompletionBlock)completionBlock;

#pragma mark create folder

/* Creates a folder at the given root/path */
- (void)createFolder:(NSString*)path withCompletionBlock:(kDBRestClientCreateFolderCompletionBlock)completionBlock;
#pragma mark delete path

- (void)deletePath:(NSString*)path withCompletionBlock:(kDBRestClientDeletePathCompletionBlock)completionBlock;

#pragma mark copy path

- (void)copyFrom:(NSString*)fromPath toPath:(NSString *)toPath withCompletionBlock:(kDBRestClientCopyPathCompletionBlock)completionBlock;

#pragma mark create copy reference

- (void)createCopyRef:(NSString *)path withCompletionBlock:(kDBRestClientCreateCopyReferenceCompletionBlock)completionBlock;

#pragma mark copy from reference to path

- (void)copyFromRef:(NSString*)copyRef toPath:(NSString *)toPath withCompletionBlock:(kDBRestClientCopyReferenceToPathCompletionBlock)completionBlock;
#pragma mark move

- (void)moveFrom:(NSString*)fromPath toPath:(NSString *)toPath withCompletionBlock:(kDBRestClientMoveCompletionBlock)completionBlock;

#pragma mark search

- (void)searchPath:(NSString*)path forKeyword:(NSString*)keyword withCompletionBlock:(kRestClientSearchCompletionBlock)completionBlock;
#pragma mark load sharable

- (void)loadSharableLinkForFile:(NSString *)path withCompletionBlock:(kDBRestClientLoadSharableLinkCompletionBlock)completionBlock;

- (void)loadSharableLinkForFile:(NSString *)path shortUrl:(BOOL)createShortUrl withCompletionBlock:(kDBRestClientLoadSharableLinkCompletionBlock)completionBlock;

#pragma mark load steamable url

- (void)loadStreamableURLForFile:(NSString *)path withCompletionBlock:(kDBRestClientLoadStreamableURLCompletionBlock)completionBlock;

@end
