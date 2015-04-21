<?php

/**
 * @file
 * KM API connection
 */

$lib = libraries_get_path('kmsuite') . '/';

require_once $lib . 'class.ecnlistmanager.php';

class ListManagerMethods extends ListManager {

  private $key;

  /**
   * Constructor.
   * 
   * @param string $key
   *   API key
   */
  public function __construct($key) {

    $this->key = $key;

    parent::__construct($this->key);

  }


  /**
   * Folder List.
   */
  public function kmsuite_list_folders() {

    $list = parent::GetFolders();

    $list = $list->xpath('//Folder');

    $options = array();
    $options[0] = 'Root';

    foreach ($list as $val) {
      $options[(int) $val->FolderID] = (string) $val->FolderName;

    }

    return $options;
  }

  /**
   * Add Folder.
   */
  public function kmsuite_add_folder($foldername, $folderdesc, $folderid) {

    $output = parent::AddFolderToParent(urlencode($foldername), urlencode($folderdesc), $folderid);

    $output = $this->kmsuite_status_message($output);

    return $output;
  }

  /**
   * Delete Folder.
   */
  public function kmsuite_delete_folder($folderid) {

    $output = parent::DeleteFolder($folderid);

    $output = $this->kmsuite_status_message($output);

    return $output;
  }

  /**
   * Lists.
   */
  public function kmsuite_lists() {

    $list = parent::GetLists();
    $list = $list->xpath('//Group');

    $options = array();
    foreach ($list as $val) {
      $options[(int) $val->GroupID] = (string) $val->GroupName;
    }

    return $options;
  }

  /**
   * Get List by Folder ID.
   */
  public function kmsuite_list_byfolderid($folderid) {

    $list = parent::GetListsByFolderID($folderid);
    $list = $list->xpath('//Group');

    $options = array();
    foreach ($list as $val) {
      $options[(int) $val->GroupID] = (string) $val->GroupName;
    }

    return $options;
  }

  /**
   * Get List By Name.
   */
  public function kmsuite_list_byname($groupname) {

    $list = parent::GetListByName($groupname);
    $list = $list->xpath('//Group');

    $options = array();
    foreach ($list as $val) {
      $options[(int) $val->GroupID] = (string) $val->GroupName;
    }

    return $options;
  }

  /**
   * Add List.
   */
  public function kmsuite_add_list($listname, $listdesc, $folderid) {

    $output = parent::AddListToFolder(urlencode($listname), urlencode($listdesc), $folderid);

    $output = $this->kmsuite_status_message($output);

    return $output;
  }

  /**
   * Delete List.
   */
  public function kmsuite_delete_list($listid) {

    $output = parent::DeleteList($listid);

    $output = $this->kmsuite_status_message($output);

    return $output;

  }

  /**
   * Update List.
   */
  public function kmsuite_update_list($listid, $listname, $listdesc, $folderid) {

    $output = parent::UpdateListWithFolder($listid, urlencode($listname), urlencode($listdesc), $folderid);

    $output = $this->kmsuite_status_message($output);

    return $output;
  }

  /**
   * Rename List.
   */
  public function kmsuite_rename_list($listid, $listname, $listdesc) {

    $output = parent::UpdateList($listid, urlencode($listname), urlencode($listdesc));

    $output = $this->kmsuite_status_message($output);

    return $output;
  }

  /**
   * Subscriber Count.
   */
  public function kmsuite_subscriber_count($listid) {

    $output = parent::GetSubscriberCount($listid);

    $output = number_format((int) $output->Outputxml);

    return $output;
  }

  /**
   * Subscriber Status.
   */
  public function kmsuite_subscriber_status($subcriber_email) {

    $output = parent::GetSubscriberStatus($subcriber_email);

    $list = $output->xpath('//Group');

    $options = array();
    foreach ($list as $val) {
      $result = $val->attributes();

      if ((string) $result->SubscriptionTypeCode == 'S') {
        $options[] = (int) $result->ID;
      }

    }

    return $options;
  }

  /**
   * Add Subscriber.
   */
  public function kmsuite_add_subscriber($listid, $subscriptiontype, $format_type, $profile) {

    $output = parent::AddSubscribers($listid, $subscriptiontype, $format_type, urlencode($profile));

    $message = $this->kmsuite_status_message($output);

    return $message;
  }

  /**
   * UnSubscribe.
   */
  public function kmsuite_subscriber_unsubscribe($listid, $subcriber_email) {

    $email = "<Emails><EmailAddress>" . $subcriber_email . "</EmailAddress></Emails>";

    $output = parent::UnsubscribeSubscriber($listid, urlencode($email));

    $output = $this->kmsuite_status_message($output);

    return $output;
  }

  /**
   * Delete Subscriber.
   */
  public function kmsuite_subscriber_delete($listid, $subcriber_email) {

    $output = parent::DeleteSubscriber($listid, $subcriber_email);

    $output = $this->kmsuite_status_message($output);

    return $output;
  }

  /**
   * Subscriber Profile.
   */
  public function kmsuite_subscriber_profile($listid, $subcriber_email) {

    $output = parent::GetListEmailProfilesByEmailAddress($listid, $subcriber_email);

    $profile = $output->xpath('//EmailProfiles');

    return $profile;

  }

  /**
   * Add Customfield.
   */
  public function kmsuite_add_customfield($listid, $customfield, $customfielddesc, $status) {

    $output = parent::AddCustomField($listid, urlencode($customfield), urlencode($customfielddesc), $status);

    $output = $this->kmsuite_status_message($output);

    return $output;
  }

  /**
   * Custom Field per list.
   */
  public function kmsuite_customfield_per_list($listid) {

    $output = parent::GetCustomFields($listid);
    $customfieldlist = $output->xpath('//CustomFields');

    $options = array();

    foreach ($customfieldlist as $val) {
      $options[(int) $val->GroupDataFieldsID] = (string) $val->ShortName;
    }

    return $options;
  }

  /**
   * Delete Custom Field.
   */
  public function kmsuite_delete_customfield($listid, $customfield) {

    $output = parent::DeleteCustomField($listid, $customfield);

    $output = $this->kmsuite_status_message($output);

    return $output;
  }

  /**
   * Update Custom Field.
   */
  public function kmsuite_update_customfield($listid, $udfid, $customfield, $customfielddesc, $status) {

    $output = parent::UpdateCustomField($listid, $udfid, urlencode($customfield), urlencode($customfielddesc), $status);

    $output = $this->kmsuite_status_message($output);

    return $output;
  }

  /**
   * API Status Messages.
   */
  public function kmsuite_status_message($output) {

    // print_r($output);

    if ($output->ErrorCode != 0) {
      $error = "Error code:" . (int) $output->ErrorCode . " " . (string) $output->Outputxml;
      $message = array('error' => $error);
    }
    elseif ((isset($output->Outputxml->New) && $output->Outputxml->New == 1) || (isset($output->Outputxml->Changed) && $output->Outputxml->Changed == 1)) {
      $message = array('status' => 'Subscriber Updated');
    }
    elseif (isset($output->Outputxml->MSSkipped) && $output->Outputxml->MSSkipped == 1) {
      $message = array('warning' => 'Subscriber email is in Master Suppression List. Can not be updated!');
    }
    else {
      $message = array('status' => (string) $output->Outputxml);
    }

    return $message;
  }

}
