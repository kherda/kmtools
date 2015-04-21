<?php

/**
 * @file
 * Knowledge Marketing Subscription Module  - Field Config File.
 */


/**
 * Custom field settings for the Fields.
 */
function kmsubscription_field_settings($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);

  if (isset($form_state['values']['op']) && $form_state['values']['op'] == 'Add Field') {
    $entries = $presets['subscription_plugin']['fields']['kmsubscription_field_count'];
    $entries++;
    $presets['subscription_plugin']['fields']['kmsubscription_field_count'] = $entries;
    variable_set('kmsuite_object', json_encode($presets));
  }
  if (isset($form_state['values']) && in_array('Delete', $form_state['values'])) {
    $remove_key = array_search('Delete', $form_state['values']);
    unset($presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['names'][$remove_key]);
    unset($presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields'][$remove_key]);
    $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['names'] = array_values($presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['names']);
    $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields'] = array_values($presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields']);
    $entries = $presets['subscription_plugin']['fields']['kmsubscription_field_count'];
    $entries--;
    $presets['subscription_plugin']['fields']['kmsubscription_field_count'] = $entries;
    variable_set('kmsuite_object', json_encode($presets));
    $form_state['input'] = array();
  }
  if (isset($form_state['values']['op']) && $form_state['values']['op'] == 'Add Type' && $form_state['values']['field_name'] != '') {
    if (in_array($form_state['values']['field_name'], $presets['subscription_plugin']['fields']['kmsubscription_fields'])) {
      drupal_set_message('The field type entered already exists.', 'error');
    }
    else {
      $options = $presets['subscription_plugin']['fields']['kmsubscription_fields'];
      $options[] = 'user_' . $form_state['values']['field_name'];
      $presets['subscription_plugin']['fields']['kmsubscription_fields'] = $options;

      variable_set('kmsuite_object', json_encode($presets));
    }
  }

  $form['km_subscriber_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subscriber Management'),
    '#description' => t('Select the available fields (user defined fields) you would like available for your subscriber.  Note: '),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="fields-fields">',
    '#suffix' => '</div>'
  );

  $rows = _build_table_array($presets);

  $form['km_subscriber_fields']['subscriber_settings_table'] = array(
    '#theme' => 'kmsubscription_list_settings_table',
    '#header' => array(
      t('Field'),
      t('Display Name'),
      t('Remove Field'),
    ),
    'rows' => $rows,
    '#prefix' => '<div id="fields-fields">',
    '#suffix' => '</div>'
  );

  $form['km_subscriber_fields']['add_field'] = array(
    '#type' => 'button',
    '#value' => 'Add Field',
    '#ajax' => array(
      'callback' => 'ajax_refresh_field',
      'wrapper' => 'fields-fields',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['km_subscriber_fields']['subscriber_add_settings_table'] = array(
    '#theme' => 'kmsubscription_list_settings_table',
    '#description' => t('Add new field types to the field selection.'),
    '#header' => array(
      t('Field Name'),
      t('Field Type Action'),
    ),
    'rows' => array(
      'row1' => array(
        'field_name' => array(
          '#type' => 'textfield',
          '#description' => t('This is your "Key" value.  This value MUST match the UDF key put into the ECN!'),
        ),
        'add_type' => array(
          '#type' => 'button',
          '#value' => 'Add Type',
          '#ajax' => array(
            'callback' => 'ajax_refresh_field',
            'wrapper' => 'fields-fields',
            'method' => 'replace',
            'effect' => 'fade',
          ),
        ),
      ),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Configuration',
    '#submit' => array('kmsubscription_field_settings_submit'),
  );

  return $form;
}

/**
 * Custom field submit handler.
 */
function kmsubscription_field_settings_submit($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);

  foreach ($form_state['values']['rows'] as $key => $value) {
    $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields'][$key] = $value['field'];
    $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['names'][$key] = $value['name'];
  }

  variable_set('kmsuite_object', json_encode($presets));
  drupal_set_message(t('Configuration settings saved.'), 'status', FALSE);
}

/**
 * Array builder.
 */
function _build_table_array($presets) {
  $rows = array('#tree' => TRUE);

  global $_options_;

  if (isset($presets['subscription_plugin']['fields']['kmsubscription_fields'])) {
    $_options_ = $presets['subscription_plugin']['fields']['kmsubscription_fields'];
  }
  else {

    // WORK
    // Need to make sure ALL keys match 100%
    // Also need to add user_ infront of UDF's
    //
    $_options_ = array(
      'EmailAddress',
      'FirstName',
      'LastName',
      'FullName',
      'Title',
      'Company',
      'Occupation',
      'Address1',
      'Address2',
      'City',
      'State',
      'Zip',
      'Voice',
      'Country',
      'Phone',
      'Mobile',
      'Fax',
      'Website',
      'Age',
      'Income',
      'Gender',
    );
    $presets['subscription_plugin']['fields']['kmsubscription_fields'] = $_options_;
    variable_set('kmsuite_object', json_encode($presets));
  }

  if (isset($presets['subscription_plugin']['fields']['kmsubscription_field_count'])) {
    $entries = $presets['subscription_plugin']['fields']['kmsubscription_field_count'];
  }
  else {
    $entries = 0;
    $presets['subscription_plugin']['fields']['kmsubscription_field_count'] = 0;
    variable_set('kmsuite_object', json_encode($presets));
  }

  for ($i = 0; $i <= $entries; $i++) {

    if (isset($presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields'][$i])) {
      $field = $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields'][$i];
    }
    else {
      $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields'][0] = 0;
      $field = $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields'][0];
    }

    if (isset($presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['names'][$i])) {
      $name = $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['names'][$i];
    }
    else {
      $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['names'][0] = 'Email Address';
      $name = $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['names'][0];;
    }

    variable_set('kmsuite_object', json_encode($presets));

    $rows[$i] = array(
      'field' => array(
        '#type' => 'select',
        '#default_value' => $field,
        '#options' => $_options_,
      ),
      'name' => array(
        '#type' => 'textfield',
        '#default_value' => $name,
      ),
      'delete' => array(
        '#type' => 'button',
        '#value' => 'Delete',
        '#name' => $i,
        '#ajax' => array(
          'callback' => 'ajax_refresh_field',
          'wrapper' => 'fields-fields',
          'method' => 'replace',
          'effect' => 'fade',
        ),
      ),
    );
  }

  return $rows;
}

/**
 * Ajax Form Rebuild.
 */
function ajax_refresh_field($form, $form_state) {

  return $form['km_subscriber_fields'];
}
