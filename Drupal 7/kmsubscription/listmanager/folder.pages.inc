<?php

/**
 * @file
 * KM Forms Folder
 */

require_once 'listmanager.api.inc';
/**
 * Add Folder Form.
 */
function kmsuite_addfolder_form($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);
  $folder_name = $lm->kmsuite_list_folders();

  $form['folder'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Folder'),
    '#description' => t('Folder creation period, by default, the folder will be added to the root unless main folder is selected.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['folder']['foldername'] = array(
    '#title' => 'Folder Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'Folder Name'),
  );

  $form['folder']['folderdesc'] = array(
    '#title' => 'Folder Decription',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'Folder Description'),
  );

  $form['folder']['folderlist'] = array(
    '#title' => 'Folder List',
    '#type' => 'select',
    '#options' => $folder_name,
  );

  $form['folder']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Add Folder',
  );

  $form['#validate'][] = 'kmsuite_addfolder_form_validate';
  $form['#submit'][] = 'kmsuite_addfolder_form_submit';

  return $form;

}

/**
 * Implementation hook_validate.
 */
function kmsuite_addfolder_form_validate($form, &$form_state) {

  $folder = $form_state['values']['foldername'];

  if (!preg_match("/^[0-9a-zA-Z ]*$/", $folder)) {
    form_set_error('error', t('Only letters, numbers and white space allowed!'));
  }
}

/**
 * Implementation hook_submit.
 */
function kmsuite_addfolder_form_submit($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);

  $output = $lm->kmsuite_add_folder(trim($form_state['values']['foldername']), trim($form_state['values']['folderdesc']), trim($form_state['values']['folderlist']));

  foreach ($output as $k => $v) {
    $status = $k;
    $message = $v;
  }
  drupal_set_message($message, $status);
}

/**
 * Delete Folder.
 */
function kmsuite_deletefolder_form($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);
  $folder_name = $lm->kmsuite_list_folders();

  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Delete Folder'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['info']['folder'] = array(
    '#title' => 'Folder List',
    '#type' => 'select',
    '#options' => $folder_name,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Delete Folder',
  );

  $form['#submit'][] = 'kmsuite_deletefolder_form_submit';

  return $form;
}
/**
 * Implementaion hook_submit.
 */
function kmsuite_deletefolder_form_submit($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);
  $output = $lm->kmsuite_delete_folder($form_state['values']['folder']);

  foreach ($output as $k => $v) {
    $status = $k;
    $message = $v;
  }
  drupal_set_message($message, $status);
}
