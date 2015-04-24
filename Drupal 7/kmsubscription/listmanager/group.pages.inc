<?php

/**
 * @file
 * KM API connection Groups
 */

require_once 'listmanager.api.inc';

/**
 * Add LIst Form.
 */
function kmsuite_addgroup_form($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);
  $folder_name = $lm->kmsuite_list_folders();

  $form['groups'] = array(
    '#type' => 'fieldset',
    '#title' => t('List Name'),
    '#description' => t('Create a list(group) in the chosen folder'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['groups']['groupname'] = array(
    '#title' => 'List (Group) Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'List Name'),
  );

  $form['groups']['groupdesc'] = array(
    '#title' => 'List Decription',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'List Description'),
  );

  $form['groups']['folderlist'] = array(
    '#title' => 'Folder List',
    '#type' => 'select',
    '#options' => $folder_name,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Add List',
  );

  $form['#validate'][] = 'kmsuite_addgroup_form_validate';
  $form['#submit'][] = 'kmsuite_addgroup_form_submit';

  return $form;

}

/**
 * Form Validate.
 */
function kmsuite_addgroup_form_validate($form, &$form_state) {

  $group = $form_state['values']['groupname'];

  if (!preg_match("/^[0-9a-zA-Z ]*$/", $group)) {
    form_set_error('error', t('Only letters, numbers and white space allowed!'));
  }

}

/**
 * Form Submit.
 */
function kmsuite_addgroup_form_submit($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);

  $output = $lm->kmsuite_add_list(trim($form_state['values']['groupname']), trim($form_state['values']['groupdesc']), $form_state['values']['folderlist']);

  foreach ($output as $k => $v) {
    $status = $k;
    $message = $v;
  }

  drupal_set_message($message, $status);
}

/**
 * Delete List Form.
 */
function kmsuite_deletegroup_form($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);

  $group_list = $lm->kmsuite_lists();

  $form['groups'] = array(
    '#type' => 'fieldset',
    '#title' => t('Delete List'),
    '#description' => t('Choose a list (group) to delete, please act with caution'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['groups']['grouplist'] = array(
    '#title' => 'Group List',
    '#type' => 'select',
    '#options' => $group_list,
  );

  $form['groups']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Delete List',
  );

  $form['#submit'][] = 'kmsuite_deletegroup_form_submit';

  return $form;
}

/**
 * Delete List Form Submit.
 */
function kmsuite_deletegroup_form_submit($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);
  $output = $lm->kmsuite_delete_list($form_state['values']['grouplist']);

  foreach ($output as $k => $v) {
    $status = $k;
    $message = $v;
  }
  drupal_set_message($message, $status);

}

/**
 * Update List Form.
 */
function kmsuite_updategroup_form($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);

  $group_list = $lm->kmsuite_lists();
  $folder_name = $lm->kmsuite_list_folders();

  $form['info'] = array(
    '#markup' => '<div class="ecn_info">Update list(group). Choose a list that you would like to update.</div>',
  );

  $form['grouplist'] = array(
    '#title' => 'Group List',
    '#type' => 'select',
    '#options' => $group_list,
  );

  $form['groupname'] = array(
    '#title' => 'List (Group) Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'List Name'),
  );

  $form['groupdesc'] = array(
    '#title' => 'List Decription',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'List Description'),
  );

  $form['folderlist'] = array(
    '#title' => 'Folder List',
    '#type' => 'select',
    '#options' => $folder_name,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Update List',
  );

  $form['#validate'][] = 'kmsuite_updategroup_form_validate';
  $form['#submit'][] = 'kmsuite_updategroup_form_submit';

  return $form;
}

/**
 * Implementation hook_validate.
 */
function kmsuite_updategroup_form_validate($form, &$form_state) {

  $group = $form_state['values']['groupname'];

  if (!preg_match("/^[0-9a-zA-Z ]*$/", $group)) {
    form_set_error('error', t('Only letters, numbers and white space allowed!'));
  }

}

/**
 * Implementation hook_submit.
 */
function kmsuite_updategroup_form_submit($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);

  $output = $lm->kmsuite_update_list($form_state['values']['grouplist'], $form_state['values']['groupname'], $form_state['values']['groupdesc'], $form_state['values']['folderlist']);

  foreach ($output as $k => $v) {
    $status = $k;
    $message = $v;
  }
  drupal_set_message($message, $status);

}

/**
 * Rename list form.
 */
function kmsuite_renamegroup_form($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);

  $group_list = $lm->kmsuite_lists();

  $form['info'] = array(
    '#markup' => '<div class="ecn_info">Choose a list(group). Type a new name and description, act with caution</div>',
  );

  $form['grouplist'] = array(
    '#title' => 'Group List',
    '#type' => 'select',
    '#options' => $group_list,
  );

  $form['groupname'] = array(
    '#title' => 'List (Group) Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'List Name'),
  );

  $form['groupdesc'] = array(
    '#title' => 'List Decription',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'List Decription'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Rename List',
  );

  $form['#validate'][] = 'kmsuite_renamegroup_form_validate';
  $form['#submit'][] = 'kmsuite_renamegroup_form_submit';

  return $form;
}


/**
 * Implementation hook_validate.
 */
function kmsuite_renamegroup_form_validate($form, &$form_state) {

  $group = $form_state['values']['groupname'];

  if (!preg_match("/^[0-9a-zA-Z ]*$/", $group)) {
    form_set_error('error', t('Only letters, numbers and white space allowed!'));
  }

}

/**
 * Implementation hook_submit.
 */
function kmsuite_renamegroup_form_submit($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);

  $output = $lm->kmsuite_rename_list($form_state['values']['grouplist'], trim($form_state['values']['groupname']), trim($form_state['values']['groupdesc']));

  foreach ($output as $k => $v) {
    $status = $k;
    $message = $v;
  }

  drupal_set_message($message, $status);

}
