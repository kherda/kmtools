<?php

/**
 * @file
 * Knowledge Marketing Subscription Module Config File.
 */

require_once 'listmanager/listmanager.api.inc';

/**
 * Form builder; Configure the knowledge marketing subscription.
 *
 * @see system_settings_form()
 */
function kmsuite_group_settings($form, $form_state) {

  $form['description'] = array(
    '#markup' => '<div>' . t('', array('!suite' => url('google.com'), '!kmcontact' => url('google.com'))) . '</div>',
  );

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);

  $groups_arr = $lm->kmsuite_lists();

  // Step 2:  Configure the options for each group.
  $form['km_group_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 2: Group Configuration'),
    '#description' => t('Each of the groups you have selected in step 1 will be available for use below.  The primary use of the fieldsets is to add an alias to the group name, description and frequency.'),
    '#collapsible' => TRUE,
    '#collapsed' => isset($presets['subscription_plugin']['groups']) ? FALSE : TRUE,
  );

  if (isset($presets['subscription_plugin']['groups'])) {
    // Gather all possible fields fields implemented for this website and present as available options to the group.
    $fields_arr = array();
    if (isset($presets['subscription_plugin']['fields']['kmsubscription_field_count'])) {
      $fields_ctr = $presets['subscription_plugin']['fields']['kmsubscription_field_count'];
    }

    for ($i = 0; $i < count(@$presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields']); $i++) {
      if (is_numeric($presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields'][$i])) {
        $fields_arr[$presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields'][$i]] = $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['names'][$i];
      }
    }

    $header = array(
      'ECN Subscriber Count',
      'Drupal Subscriber Count',
      'Used Locations',
    );
    $frequency = array(
      'daily' => 'Daily',
      'biweekly' => 'Bi-Weekly',
      'weekly' => 'Weekly',
      'monthly' => 'Monthly',
      'yearly' => 'Yearly'
    );

    // Initiate loop to iterate through saved groups.
    foreach ($presets['subscription_plugin']['groups'] as $k => $v) {

      $form['km_group_details']['km_saved_groups']['fieldset_' . $k] = array(
        '#type' => 'fieldset',
        '#title' => t($groups_arr[$k]),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      // Update:  Cache requests to the API incase user loads multiple lists (each API call decreases page load significantly).
      $api_count = NULL;
      if ($presets['subscription_plugin']['groups'][$k]['apitimestamp'] == NULL || $presets['subscription_plugin']['groups'][$k]['apitimestamp'] >= strtotime('+1 day', time())) {
        $api_count = $lm->kmsuite_subscriber_count($k);
      }
      else {
        $api_count = $presets['subscription_plugin']['groups'][$k]['apicount'];
      }

      $total = db_query("SELECT COUNT(action) as total FROM kmsubscription_data WHERE action = 'Subscribed' AND group_id = " . $k)->fetchAll();

      $used_location = "";

      if (isset($presets['subscription_plugin']['block']) && $presets['subscription_plugin']['block']['groups'][$k]['enabled'] == 1) {
        $used_location .= "Block ";
      }

      if (isset($presets['subscription_plugin']['page']) && $presets['subscription_plugin']['page']['groups'][$k]['enabled'] == 1) {
        $used_location .= "Page ";
      }

      if (isset($presets['overlay_plugin']['overlay_groups'][$k]) && $presets['overlay_plugin']['overlay_groups'][$k] != 0) {
        $used_location .= "Overlay ";
      }

      $rows = array();
      $rows[] = array(
        $api_count,
        $total[0]->total,
        $used_location,
      );

      $form['km_group_details']['km_saved_groups']['fieldset_' . $k]['table_' . $k] = array(
        '#type' => 'markup',
        '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
      );

      $form['km_group_details']['km_saved_groups']['fieldset_' . $k]['apitimestamp_' . $k] = array(
        '#type' => 'hidden',
        '#default_value' => $presets['subscription_plugin']['groups'][$k]['apitimestamp'] ? $presets['subscription_plugin']['groups'][$k]['apitimestamp'] : time(),
      );
      $form['km_group_details']['km_saved_groups']['fieldset_' . $k]['apicount_' . $k] = array(
        '#type' => 'hidden',
        '#default_value' => $api_count,
      );
      $form['km_group_details']['km_saved_groups']['fieldset_' . $k]['origname_' . $k] = array(
        '#type' => 'hidden',
        '#default_value' => $groups_arr[$k],
      );

      $form['km_group_details']['km_saved_groups']['fieldset_' . $k]['alias_' . $k] = array(
        '#type' => 'textfield',
        '#title' => t('Group Alias'),
        '#default_value' => $presets['subscription_plugin']['groups'][$k]['alias'],
        '#description' => t('The alias will be shown to the public for group selection.'),
      );
      $form['km_group_details']['km_saved_groups']['fieldset_' . $k]['description_' . $k] = array(
        '#type' => 'textarea',
        '#title' => t('Group Description'),
        '#default_value' => $presets['subscription_plugin']['groups'][$k]['desc'],
        '#description' => t('The optional discription will be shown on the subscription landing page.'),
      );
      $form['km_group_details']['km_saved_groups']['fieldset_' . $k]['frequency_' . $k] = array(
        '#type' => 'select',
        '#title' => t('Frequency'),
        '#default_value' => $presets['subscription_plugin']['groups'][$k]['frequency'],
        '#options' => $frequency,
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Configuration',
    '#submit' => array('kmsuite_group_settings_submit'),
  );

  return $form;
}

/**
 * Custom submit handler for the config settings.
 *
 * @todo Ensure group origname is available.  Currently only works on second submit.
 */
function kmsuite_group_settings_submit($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $groups = $presets['subscription_plugin']['groups'];

  foreach ($form_state['values'] as $k => $v) {

    if (stristr($k, 'origname')) {

      $key = explode('_', $k);
      $key = $key[1];

      if (array_key_exists($key, $groups)) {
        $presets['subscription_plugin']['groups'][$key]['origname'] = $v;
      }
    }
    if (stristr($k, 'alias')) {

      $key = explode('_', $k);
      $key = $key[1];

      if (array_key_exists($key, $groups)) {
        $presets['subscription_plugin']['groups'][$key]['alias'] = $v;
      }
    }

    if (stristr($k, 'description')) {
      $key = explode('_', $k);
      $key = $key[1];
      if (array_key_exists($key, $groups)) {
        $presets['subscription_plugin']['groups'][$key]['desc'] = $v;
      }
    }

    if (stristr($k, 'frequency')) {
      $key = explode('_', $k);
      $key = $key[1];
      if (array_key_exists($key, $groups)) {
        $presets['subscription_plugin']['groups'][$key]['frequency'] = $v;
      }
    }

    if (stristr($k, 'apitimestamp')) {
      $key = explode('_', $k);
      $key = $key[1];
      if (array_key_exists($key, $groups)) {
        $presets['subscription_plugin']['groups'][$key]['apitimestamp'] = $v;
      }
    }

    if (stristr($k, 'apicount')) {
      $key = explode('_', $k);
      $key = $key[1];
      if (array_key_exists($key, $groups)) {
        $presets['subscription_plugin']['groups'][$key]['apicount'] = $v;
      }
    }

    if (stristr($k, 'fields')) {
      $key = explode('_', $k);
      $key = $key[1];

      // Saved Fields that the user wants to send over.
      if (array_key_exists($key, $groups)) {
        $presets['subscription_plugin']['groups'][$key]['fields'] = $v;
      }
    }
  }

  variable_set('kmsuite_object', json_encode($presets));
  drupal_set_message(t('Configuration settings saved.'), 'status', FALSE);
}

/**
 * Form builder; Configure the knowledge marketing subscription.
 *
 * @see system_settings_form()
 */
function kmsuite_group_selection($form, $form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);

  $lm = new ListManagerMethods($presets['config']['token']);
  $groups_arr = $lm->kmsuite_lists();

  $form['km_group_selection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 1: Group Selection'),
    '#description' => t('Below you should find all groups associated with your ECN account (determined from your token).  <br />Select the group(s) you would like to use throughout the website.  You can configure a friendly alias after the group selection. <br /><br /><i>Note: after selecting the group, please go to Step 2, in the tabs.</i>'),
    '#collapsible' => TRUE,
    '#collapsed' => (isset($presets['subscription_plugin']['groups']) && count($presets['subscription_plugin']['groups']) > 0) ? FALSE : TRUE,
  );

  $form['km_group_selection']['km_all_groups'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('List Selection'),
    '#description' => t('Group Selection:  You can select as many groups as you would like for your werbsite to use.'),
    '#default_value' => isset($presets['subscription_plugin']['groups']) ? array_keys($presets['subscription_plugin']['groups']) : NULL,
    '#options' => $groups_arr,
    '#size' => 20,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Configuration',
    '#submit' => array('kmsuite_group_selection_submit'),
  );

  return $form;

}

/**
 * Custom submit handler for the config settings.
 */
function kmsuite_group_selection_submit($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  $lm = new ListManagerMethods($presets['config']['token']);
  $groups_arr = $lm->kmsuite_lists();

  unset($presets['subscription_plugin']['groups']);

  foreach ($form_state['values']['km_all_groups'] as $group) {
    $presets['subscription_plugin']['groups'][$group] = array(
      'origname' => $groups_arr[$group],
      'alias' => NULL,
      'desc' => NULL,
      'frequency' => NULL,
      'fields' => NULL,
      'apitimestamp' => NULL,
      'apicount' => NULL,
    );
  }

  variable_set('kmsuite_object', json_encode($presets));

  $diff_page = array_diff_key($presets['subscription_plugin']['page']['groups'], $presets['subscription_plugin']['groups']);
  $diff_block = array_diff_key($presets['subscription_plugin']['block']['groups'], $presets['subscription_plugin']['groups']);

  foreach ($diff_page as $k => $v) {
    unset($presets['subscription_plugin']['page']['groups'][$k]);
  }

  foreach ($diff_block as $k => $v) {
    unset($presets['subscription_plugin']['block']['groups'][$k]);
  }

  foreach ($presets['subscription_plugin']['groups'] as $k => $v) {

    if (!array_key_exists($k, $presets['subscription_plugin']['page']['groups'])) {
      $presets['subscription_plugin']['page']['groups'][$k] = array('weight' => 0, 'enabled' => 0);
    }

    if (!array_key_exists($k, $presets['subscription_plugin']['block']['groups'])) {
      $presets['subscription_plugin']['block']['groups'][$k] = array('weight' => 0, 'enabled' => 0);
    }

  }

  variable_set('kmsuite_object', json_encode($presets));

  drupal_set_message(t('Configuration settings saved.'), 'status', FALSE);

}
