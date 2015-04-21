<?php
/**
 *  ECN List Manager
 *
 *  The ECN List Manager is a set of API calls used to fully manage your
 *  Folders, lists and subscriber based information in the ECN.
 *
 * @package    ECN Suite
 * @author     Kevin Herda <kherda@sgcmail.com.com>
 * @version    Release: 1.2
 */

require('class.ecncommunicator.php');

/**
 * The List Manager Class is used to push and get information from the ListManager API section in the ECN.
 *
 * Classes for the ListManager can be found from the URL below:
 * http://webservices.ecn5.com/ListManager.asmx
 *
 * Note:  The function naming convention must match the API function naming method exactly.
 */
class ListManager extends Communicator {

  /**
   * The token is the authentication key used to validate the API request.
   * @var String
   */
  protected $token;

  /**
   * Portal
   *
   * The portal will either be ECN or UAD.
   * @var  String
   */
  protected $portal;

  /**
   * The keyname is slightly different accross a few API calls, and definitely different from ECN to UAD.
   * @var string
   */
  protected $keyname = 'ecnAccessKey';

  /**
   * Manager for use in the Communicator
   * @var string
   */
  protected $manager = __CLASS__;

  /**
   * Extension for ECN API Calls
   */
  protected $ext = '.asmx/';

  /**
   * Addional slash for UAD Param
   * @var string
   */
  protected $slash = NULL;

  /**
   * Extraction
   *
   * The extraction is how we will render the output of the API Call.
   * @var String.
   */
  protected $extraction;

  /**
   * Construct the protected elements of the class.
   * @param String $token
   */
  public function __construct($token, $extraction = 'array') {

    $this->token = $token;
    $this->portal = 'ECN';
    $this->extraction = $extraction;
  }

  /**
   * [AddCustomField]
   * @param int    $listID
   * @param string $customFieldName
   * @param string $customFieldDescription
   * @param bool   $isPublic - 'N' or 'Y'
   */
  public function AddCustomField($listID, $customFieldName, $customFieldDescription, $isPublic = 'N') {

    $params = "&listID={$listID}&customFieldName={$customFieldName}&customFieldDescription={$customFieldDescription}&isPublic={$isPublic}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * Add Folder
   * @param String $folderName
   * @param String $folderDescription
   */
  public function AddFolder($folderName, $folderDescription){

  }

  /**
   * Add Folder to Parent
   *
   * Note: Use this API call as we can restrict the Parent to NULL if needs be.  AddFolder is irrelevant.
   *
   * @param String $folderName
   * @param String $folderDescription
   * @param int    $parentFolderID
   */
  public function AddFolderToParent($folderName, $folderDescription, $parentFolderID) {

    $params = "&folderName={$folderName}&folderDescription={$folderDescription}&parentFolderID={$parentFolderID}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * Add List
   *
   * @param String $listName
   * @param String $listDescription
   */
  public function AddList($listName, $listDescription){

  }

  /**
   * Add List
   *
   * Note: Use this API call as we can restrict the Parent to NULL if needs be.  AddList is irrelevant.
   *
   * @param String $listName
   * @param String $listDescription
   * @param Int    $FolderID
   */
  public function AddListToFolder($listName, $listDescription, $FolderID) {

    $params = "&listName={$listName}&listDescription={$listDescription}&FolderID={$FolderID}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * [AddSubscriberUsingSmartForm]
   * @param int $listID
   * @param [type] $subscriptionType
   * @param [type] $formatType
   * @param [type] $xmlString
   * @param [type] $sfID
   */
  public function AddSubscriberUsingSmartForm($listID, $subscriptionType, $formatType, $xmlString, $sfID) {

    $param = "&listID={$listID}&subscriptionType={$subscriptionType}&formatType={$formatType}&xmlString={$xmlString}&sfID=$sfID";

    return parent::execute(__FUNCTION__, $params);

  }

  /**
   * [AddSubscribers description]
   * @param int $listID
   * @param [type] $subscriptionType - S/U
   * @param [type] $formatType - 'html'/'text'
   * @param [type] $xmlString
   */
  public function AddSubscribers($listID, $subscriptionType = 'S', $formatType = 'html', $xmlString){

    $params = "&listID={$listID}&subscriptionType={$subscriptionType}&formatType={$formatType}&xmlString={$xmlString}";

    return parent::execute(__FUNCTION__, $params);

  }

  /**
   * [AddSubscribersWithDupes description]
   * @param [type] $listID
   * @param [type] $subscriptionType
   * @param [type] $formatType
   * @param [type] $compositeKey
   * @param [type] $overwriteWithNULL
   * @param [type] $xmlString
   */
  public function AddSubscribersWithDupes($listID, $subscriptionType, $formatType, $compositeKey, $overwriteWithNULL, $xmlString){

    $params = "&listID={$listID}&subscriptionType={$subscriptionType}&formatType={$formatType}&compositeKey={$compositeKey}&overwriteWithNULL={$overwriteWithNULL}&xmlString={$xmlString}";

    return parent::execute(__FUNCTION__, $params);

  }

  /**
   * [AddSubscribersWithDupesUsingSmartForm description]
   * @param [type] $listID
   * @param [type] $subscriptionType
   * @param [type] $formatType
   * @param [type] $compositeKey
   * @param [type] $overwriteWithNULL
   * @param [type] $xmlString
   * @param [type] $SFID
   */
  public function AddSubscribersWithDupesUsingSmartForm($listID, $subscriptionType, $formatType, $compositeKey, $overwriteWithNULL, $xmlString, $SFID){

    $params = "&listID={$listID}&subscriptionType={$subscriptionType}&formatType={$formatType}&compositeKey={$compositeKey}&overwriteWithNULL={$overwriteWithNULL}&xmlString={$xmlString}&SFID={$SFID}";

    return parent::execute(__FUNCTION__, $params);

  }

  /**
   * [AddToMasterSuppressionList]
   * @param string $xmlString
   */
  public function AddToMasterSuppressionList($xmlString){

    $params = "&xmlString={$xmlString}";

    print_r($params);

    return parent::execute(__FUNCTION__, $params);

  }

  /**
   * [DeleteCustomField]
   * @param int $ListID
   * @param int $UDFID  User Defined Field
   */
  public function DeleteCustomField($ListID, $UDFID){

    $params = "&ListID={$ListID}&UDFID={$UDFID}";

    return parent::execute(__FUNCTION__, $params);

  }

  /**
   * [DeleteFolder]
   * @param int $FolderID
   */
  public function DeleteFolder($FolderID){

    $params = "&FolderID={$FolderID}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * [DeleteList]
   * @param int $ListID
   */
  public function DeleteList($ListID){

    $params = "&ListID={$ListID}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * [DeleteList]
   * @param int $ListID
   */
  public function DeleteSubscriber($ListID, $EmailAddress){

    $params = "&ListID={$ListID}&EmailAddress={$EmailAddress}";

    return parent::execute(__FUNCTION__, $params);

  }

  /**
   * [GetCustomFields]
   * @param int $listID
   */
  public function GetCustomFields($listID){

    $params = "&listID={$listID}";

    return parent::execute(__FUNCTION__, $params);

  }

  /**
   * [GetFilters description]
   * @param int $listID
   */
  public function GetFilters($listID){

    $params = "&listID={$listID}";

    return parent::execute(__FUNCTION__, $params);

  }

  /**
   * [GetFolders]
   */
  public function GetFolders() {

    $params = NULL;

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * [GetListByName]
   * @param string $Name
   */
  public function GetListByName($Name){

    $params = "&Name={$Name}";

    return parent::execute(__FUNCTION__, $params);

  }

  /**
   * [GetListEmailProfilesByEmailAddress description]
   * @param int $listID
   * @param string $emailAddress
   */
  public function GetListEmailProfilesByEmailAddress($listID, $emailAddress) {

    $params = "&listID={$listID}&emailAddress={$emailAddress}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * GetLists - Get List- Group Name and Group ID
   */
  public function GetLists() {

    $params = NULL;

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * [GetSubscriberCount by GroupID - ListID]
   * @param int $GroupID
   */
  public function GetSubscriberCount($GroupID) {

    $params = "&GroupID={$GroupID}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * [GetSubscriberStatus description]
   * @param string $emailAddress
   */
  public function GetSubscriberStatus($emailAddress) {

    $params ="&emailAddress={$emailAddress}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * [UnsubscribeSubscriber description]
   * @param int $listID
   * @param string $XMLEmails  xml
   */
  public function UnsubscribeSubscriber($listID, $XMLEmails) {

    $params = "&listID={$listID}&XMLEmails={$XMLEmails}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * [UpdateCustomField description]
   * @param int $listID
   * @param int $udfID
   * @param string $customFieldName
   * @param string $customFieldDescription
   * @param string $isPublic - 'N' or 'Y'
   */
  public function UpdateCustomField($listID, $udfID, $customFieldName, $customFieldDescription, $isPublic = 'N') {

    $params = "&listID={$listID}&udfID={$udfID}&customFieldName={$customFieldName}&customFieldDescription={$customFieldDescription}&isPublic={$isPublic}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * [UpdateEmailAddress description]
   * @param int $ecn_listID
   * @param string $ecn_emailProfilesXMLString
   * @param string $oldEmailAddress
   * @param string $newEmailAddress
   * @param int $sfID
   */
  public function UpdateEmailAddress($ecn_listID, $ecn_emailProfilesXMLString, $oldEmailAddress, $newEmailAddress, $sfID) {

    $this->keyname = "ecn_accessKey";

    $params = "&ecn_listID={$ecn_listID}&ecn_emailProfilesXMLString={$ecn_emailProfilesXMLString}&oldEmailAddress={$oldEmailAddress}&newEmailAddress={$newEmailAddress}&sfID={$sfID}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * [UpdateList]
   * @param [int] $ListID
   * @param [string] $NewListName
   * @param [string] $NewListDescription
   */
  public function UpdateList($ListID, $NewListName, $NewListDescription) {

    $params = "&ListID={$ListID}&NewListName={$NewListName}&NewListDescription={$NewListDescription}";

    return parent::execute(__FUNCTION__, $params);
  }

  /**
   * [UpdateListWithFolder  - assign list to different folder]
   * @param [int] $ListID
   * @param [string] $NewListName
   * @param [string] $NewListDescription
   * @param [int] $NewFolderID
   */
  public function UpdateListWithFolder($ListID, $NewListName, $NewListDescription, $NewFolderID) {

    $params = "&ListID={$ListID}&NewListName={$NewListName}&NewListDescription={$NewListDescription}&NewFolderID={$NewFolderID}";

    return parent::execute(__FUNCTION__, $params);
  }
}
