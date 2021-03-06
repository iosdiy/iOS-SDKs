//
//  CKSubscription.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CloudKit/CKDefines.h>
#import <CloudKit/CKRecord.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, CKSubscriptionType) {
    CKSubscriptionTypeQuery                                     = 1,
    CKSubscriptionTypeRecordZone                                = 2,
    CKSubscriptionTypeDatabase API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0))   = 3,
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(6.0));

@class CKNotificationInfo, CKRecordZoneID;

typedef NSString *CKSubscriptionID;

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKSubscription : NSObject <NSSecureCoding, NSCopying>

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property (nonatomic, readonly, copy) CKSubscriptionID subscriptionID API_AVAILABLE(watchos(6.0));
@property (nonatomic, readonly, assign) CKSubscriptionType subscriptionType API_AVAILABLE(watchos(6.0));

/*! @abstract Describes the notification that will be sent when the subscription fires.
 *
 *  @discussion This property must be set to a non-nil value before saving the `CKSubscription`.
 */
@property (nonatomic, copy, nullable) CKNotificationInfo *notificationInfo API_AVAILABLE(watchos(6.0));

@end


typedef NS_OPTIONS(NSUInteger, CKQuerySubscriptionOptions) {
    CKQuerySubscriptionOptionsFiresOnRecordCreation     = 1 << 0,
    CKQuerySubscriptionOptionsFiresOnRecordUpdate       = 1 << 1,
    CKQuerySubscriptionOptionsFiresOnRecordDeletion     = 1 << 2,
    CKQuerySubscriptionOptionsFiresOnce                 = 1 << 3,
} API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(6.0));

/*! @class CKQuerySubscription
 *
 *  @abstract A subscription that fires whenever a change matching the predicate occurs.
 *
 *  @discussion `CKQuerySubscriptions` are not supported in a `sharedCloudDatabase`
 */
API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(6.0))
@interface CKQuerySubscription : CKSubscription <NSSecureCoding, NSCopying>

- (instancetype)initWithRecordType:(CKRecordType)recordType predicate:(NSPredicate *)predicate options:(CKQuerySubscriptionOptions)querySubscriptionOptions;
- (instancetype)initWithRecordType:(CKRecordType)recordType predicate:(NSPredicate *)predicate subscriptionID:(CKSubscriptionID)subscriptionID options:(CKQuerySubscriptionOptions)querySubscriptionOptions NS_DESIGNATED_INITIALIZER;

/*! The record type that this subscription watches */
@property (nonatomic, readonly, copy) CKRecordType recordType;

/*! A predicate that determines when the subscription fires. */
@property (nonatomic, readonly, copy) NSPredicate *predicate;

/*! Optional property.  If set, a query subscription is scoped to only record changes in the indicated zone. */
@property (nonatomic, copy, nullable) CKRecordZoneID *zoneID;

/*! @abstract Options flags describing the firing behavior subscription.
 *
 *  @discussion One of
 *  `CKQuerySubscriptionOptionsFiresOnRecordCreation`,
 *  `CKQuerySubscriptionOptionsFiresOnRecordUpdate`, or
 *  `CKQuerySubscriptionOptionsFiresOnRecordDeletion` must be specified or an `NSInvalidArgumentException` will be thrown.
 */
@property (nonatomic, readonly, assign) CKQuerySubscriptionOptions querySubscriptionOptions;

@end


/*! @class CKRecordZoneSubscription
 *
 *  @abstract A subscription that fires whenever any change happens in the indicated Record Zone.
 *
 *  @discussion The RecordZone must have the capability `CKRecordZoneCapabilityFetchChanges`
 *  `CKRecordZoneSubscriptions` are not supported in a `sharedCloudDatabase`
 */
API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(6.0))
@interface CKRecordZoneSubscription : CKSubscription <NSSecureCoding, NSCopying>

- (instancetype)initWithZoneID:(CKRecordZoneID *)zoneID;
- (instancetype)initWithZoneID:(CKRecordZoneID *)zoneID subscriptionID:(CKSubscriptionID)subscriptionID NS_DESIGNATED_INITIALIZER;

@property (nonatomic, readonly, copy) CKRecordZoneID *zoneID;

/*! Optional property. If set, a zone subscription is scoped to record changes for this record type */
@property (nonatomic, copy, nullable) CKRecordType recordType;

@end


/*! @class CKDatabaseSubscription
 *
 *  @abstract A subscription fires whenever any change happens in the database that this subscription was saved in.
 *
 *  @discussion `CKDatabaseSubscription` is only supported in the Private and Shared databases.
 */
API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(6.0))
@interface CKDatabaseSubscription : CKSubscription <NSSecureCoding, NSCopying>

- (instancetype)init;
+ (instancetype)new OBJC_SWIFT_UNAVAILABLE("use object initializers instead");
- (instancetype)initWithSubscriptionID:(CKSubscriptionID)subscriptionID NS_DESIGNATED_INITIALIZER;

/*! Optional property. If set, a database subscription is scoped to record changes for this record type */
@property (nonatomic, copy, nullable) CKRecordType recordType;

@end


/*! @class CKNotificationInfo
 *
 *  @discussion The payload of a push notification delivered in the UIApplication `application:didReceiveRemoteNotification:` delegate method contains information about the firing subscription.
 *
 *  Use `+[CKNotification notificationFromRemoteNotificationDictionary:]` to parse that payload.
 *  On tvOS, alerts, badges, sounds, and categories are not handled in push notifications. However, CKSubscriptions remain available to help you avoid polling the server.
 */
API_AVAILABLE(macos(10.10), ios(8.0), watchos(6.0))
@interface CKNotificationInfo : NSObject <NSSecureCoding, NSCopying>

/*! Optional alert string to display in a push notification. */
@property (nonatomic, copy, nullable) NSString *alertBody __TVOS_PROHIBITED;

/*! Instead of a raw alert string, you may optionally specify a key for a localized string in your app's Localizable.strings file. */
@property (nonatomic, copy, nullable) NSString *alertLocalizationKey __TVOS_PROHIBITED;

/*! A list of field names to take from the matching record that is used as substitution variables in a formatted alert string. */
@property (nonatomic, copy, nullable) NSArray<CKRecordFieldKey> *alertLocalizationArgs __TVOS_PROHIBITED;

/*! Optional title of the alert to display in a push notification. */
@property (nonatomic, copy, nullable) NSString *title API_AVAILABLE(macos(10.13), ios(11.0)) __TVOS_PROHIBITED;

/*! Instead of a raw title string, you may optionally specify a key for a localized string in your app's Localizable.strings file. */
@property (nonatomic, copy, nullable) NSString *titleLocalizationKey API_AVAILABLE(macos(10.13), ios(11.0)) __TVOS_PROHIBITED;

/*! A list of field names to take from the matching record that is used as substitution variables in a formatted title string. */
@property (nonatomic, copy, nullable) NSArray<CKRecordFieldKey> *titleLocalizationArgs API_AVAILABLE(macos(10.13), ios(11.0)) __TVOS_PROHIBITED;

/*! Optional subtitle of the alert to display in a push notification. */
@property (nonatomic, copy, nullable) NSString *subtitle API_AVAILABLE(macos(10.13), ios(11.0)) __TVOS_PROHIBITED;

/*! Instead of a raw subtitle string, you may optionally specify a key for a localized string in your app's Localizable.strings file. */
@property (nonatomic, copy, nullable) NSString *subtitleLocalizationKey API_AVAILABLE(macos(10.13), ios(11.0)) __TVOS_PROHIBITED;

/*! A list of field names to take from the matching record that is used as substitution variables in a formatted subtitle string. */
@property (nonatomic, copy, nullable) NSArray<CKRecordFieldKey> *subtitleLocalizationArgs API_AVAILABLE(macos(10.13), ios(11.0)) __TVOS_PROHIBITED;

/*! A key for a localized string to be used as the alert action in a modal style notification. */
@property (nonatomic, copy, nullable) NSString *alertActionLocalizationKey __TVOS_PROHIBITED;

/*! The name of an image in your app bundle to be used as the launch image when launching in response to the notification. */
@property (nonatomic, copy, nullable) NSString *alertLaunchImage __TVOS_PROHIBITED;

/*! The name of a sound file in your app bundle to play upon receiving the notification. */
@property (nonatomic, copy, nullable) NSString *soundName __TVOS_PROHIBITED;

/*! @abstract A list of keys from the matching record to include in the notification payload.
 *
 *  @discussion Only some keys are allowed.  The value types associated with those keys on the server must be one of these classes:
 *  - CKReference
 *  - CLLocation
 *  - NSDate
 *  - NSNumber
 *  - NSString
 */
@property (nonatomic, copy, nullable) NSArray<CKRecordFieldKey> *desiredKeys;

/*! Indicates that the notification should increment the app's badge count. Default value is NO. */
@property (nonatomic, assign) BOOL shouldBadge API_AVAILABLE(tvos(10.0));

/*! @abstract Indicates that the notification should be sent with the "content-available" flag to allow for background downloads in the application.
 *
 *  @discussion Default value is NO.
 */
@property (nonatomic, assign) BOOL shouldSendContentAvailable;

/*! @abstract Indicates that the notification should be sent with the "mutable-content" flag to allow a Notification Service app extension to modify or replace the push payload.
 *
 *  @discussion Default value is NO.
 */
@property (nonatomic, assign) BOOL shouldSendMutableContent API_AVAILABLE(macos(10.13), ios(11.0), tvos(11.0));


/*! @abstract Optional property for the category to be sent with the push when this subscription fires.
 *
 *  @discussion Categories allow you to present custom actions to the user on your push notifications.
 *
 *  @see UIMutableUserNotificationCategory
 */
@property (nonatomic, copy, nullable) NSString *category API_AVAILABLE(macos(10.11), ios(9.0)) __TVOS_PROHIBITED;

/*! @abstract Optional property specifying a field name to take from the matching record whose value is used as the apns-collapse-id header.
 *
 *  @see APNs Notification API documentation
 */
@property (nonatomic, copy, nullable) NSString *collapseIDKey API_AVAILABLE(macos(10.13), ios(11.0), tvos(11.0));

@end

#pragma mark - Deprecated CKSubscription

/*! Replaced with CKQuerySubscriptionOptions */
typedef NS_OPTIONS(NSUInteger, CKSubscriptionOptions) {
    CKSubscriptionOptionsFiresOnRecordCreation     = 1 << 0,
    CKSubscriptionOptionsFiresOnRecordUpdate       = 1 << 1,
    CKSubscriptionOptionsFiresOnRecordDeletion     = 1 << 2,
    CKSubscriptionOptionsFiresOnce                 = 1 << 3,
} API_DEPRECATED("Use CKQuerySubscriptionOptions instead", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0)) __WATCHOS_PROHIBITED;

@interface CKSubscription (CKSubscriptionDeprecated)

- (instancetype)initWithCoder:(NSCoder *)aDecoder API_DEPRECATED("Init the appropriate CKSubscription subclass", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0)) __WATCHOS_PROHIBITED;

/*! Replaced with CKQuerySubscription */
- (instancetype)initWithRecordType:(CKRecordType)recordType predicate:(NSPredicate *)predicate options:(CKSubscriptionOptions)subscriptionOptions API_DEPRECATED("Use CKQuerySubscription instead", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0)) __WATCHOS_PROHIBITED;
- (instancetype)initWithRecordType:(CKRecordType)recordType predicate:(NSPredicate *)predicate subscriptionID:(CKSubscriptionID)subscriptionID options:(CKSubscriptionOptions)subscriptionOptions API_DEPRECATED("Use CKQuerySubscription instead", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0)) __WATCHOS_PROHIBITED;
@property (nonatomic, readonly, copy, nullable) CKRecordType recordType API_DEPRECATED("Use CKQuerySubscription instead", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0), watchos(6.0, 6.0));
@property (nonatomic, readonly, copy, nullable) NSPredicate *predicate API_DEPRECATED("Use CKQuerySubscription instead", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0)) __WATCHOS_PROHIBITED;

/*! Replaced with CKQuerySubscriptionOptions */
@property (nonatomic, readonly, assign) CKSubscriptionOptions subscriptionOptions API_DEPRECATED("Use CKQuerySubscriptionOptions instead", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0)) __WATCHOS_PROHIBITED;

/*! Replaced with CKRecordZoneSubscription */
- (instancetype)initWithZoneID:(CKRecordZoneID *)zoneID options:(CKSubscriptionOptions)subscriptionOptions API_DEPRECATED("Use CKRecordZoneSubscription instead", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0)) __WATCHOS_PROHIBITED;
- (instancetype)initWithZoneID:(CKRecordZoneID *)zoneID subscriptionID:(CKSubscriptionID)subscriptionID options:(CKSubscriptionOptions)subscriptionOptions API_DEPRECATED("Use CKRecordZoneSubscription instead", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0)) __WATCHOS_PROHIBITED;
@property (nonatomic, copy, nullable) CKRecordZoneID *zoneID API_DEPRECATED("Use CKRecordZoneSubscription instead", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0)) __WATCHOS_PROHIBITED;

@end


NS_ASSUME_NONNULL_END
