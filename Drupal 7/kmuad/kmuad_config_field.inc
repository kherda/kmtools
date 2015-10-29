<?php

/**
 * @file
 * Knowledge Marketing Subscription Module  - Field Config File.
 */


/**
 * Custom field settings for the Fields.
 */
function kmuad_field_settings($form, &$form_state) {

  $presets = json_decode(variable_get('kmuad_object'), TRUE);
  // Echo "<pre>"; print_r($presets);

  if (empty($form_state['entries'])) {

    if (isset($presets['uad']['fields']['entries'])) {
      $form_state['entries'] = $presets['uad']['fields']['entries'];
      $entries = $form_state['entries'];
    }
    else {
      $form_state['entries'] = 1;
      $entries = $form_state['entries'];
    }
  }
  else {
    $entries = $form_state['entries'];
  }

  $form['kmuad_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('UAD Fields'),
    '#description' => t('You can use the tokens below for the fields you would like to reference throughout your website.  An example would be if you wanted to showcase the users job title.  Select the token from below (ex. [kmdemographics:job-title]) and give it a frieldy name.<br /><br />'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="fields-fields">',
    '#suffix' => '</div>'
  );

  $rows = array('#tree' => TRUE);

  for ($i = 0; $i < $entries; $i++) {

    $name = isset($presets['uad']['fields']['kmuad_saved_fields']['names'][$i]) ? $presets['uad']['fields']['kmuad_saved_fields']['names'][$i] : '';
    $uad_token = isset($presets['uad']['fields']['kmuad_saved_fields']['uad_token'][$i]) ? $presets['uad']['fields']['kmuad_saved_fields']['uad_token'][$i] : '';

    variable_set('kmuad_object', json_encode($presets));

    $rows[$i] = array(
      'name' => array(
        '#type' => 'textfield',
        '#default_value' => $name,
      ),
      'uad_token' => array(
        '#type' => 'textfield',
        '#default_value' => $uad_token,
      ),
      'delete' => array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#submit' => array('kmuad_remove_one'),
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

  $form['kmuad_fields']['subscriber_settings_table'] = array(
    '#theme' => 'kmuad_list_settings_table',
    '#header' => array(
      t('Display Name'),
      t('Token'),
      t('Remove Field'),
    ),
    'rows' => $rows,
    '#prefix' => '<div id="fields-fields">',
    '#suffix' => '</div>'
  );

  $form['kmuad_fields']['add_field'] = array(
    '#type' => 'submit',
    '#value' => 'Add Field',
    '#submit' => array('kmuad_add_one'),
    '#ajax' => array(
      'callback' => 'ajax_refresh_field',
      'wrapper' => 'fields-fields',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  // Token tree display STARTS.
  $token_types = array();
  if (module_exists('token')) {
    $form['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => $token_types,
    );
  }
  else {
    $form['token_tree'] = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view the available token browser.', array('@drupal-token' => 'http://drupal.org/project/token')) . '</p>',
    );
  }
  // Token tree display ENDS.

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Configuration',
    '#submit' => array('kmuad_field_settings_submit'),
  );

  return $form;
}

/**
 * Implements kmuad_add_one.
 */
function kmuad_add_one($form, &$form_state) {
  $form_state['entries']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Implements kmuad_remove_one.
 */
function kmuad_remove_one($form, &$form_state) {
  if ($form_state['entries'] > 1) {
    $form_state['entries']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Custom field submit handler.
 */
function kmuad_field_settings_submit($form, &$form_state) {

  $presets = json_decode(variable_get('kmuad_object'), TRUE);

  unset($presets['uad']['fields']['kmuad_saved_fields']);
  foreach ($form_state['values']['rows'] as $key => $value) {
    $presets['uad']['fields']['kmuad_saved_fields']['names'][$key] = $value['name'];
    $presets['uad']['fields']['kmuad_saved_fields']['uad_token'][$key] = $value['uad_token'];
  }

  $presets['uad']['fields']['entries'] = count($form_state['values']['rows']);

  variable_set('kmuad_object', json_encode($presets));
  drupal_set_message(t('Configuration settings saved.'), 'status', FALSE);
}

/**
 * Ajax Form Rebuild.
 */
function ajax_refresh_field($form, $form_state) {

  return $form['kmuad_fields'];
}
