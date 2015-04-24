<?php

/**
 * @file
 * Knowledge Marketing Subscription Module Config File.
 */

/**
 * Form builder; Configure the knowledge marketing subscription.
 *
 * @see system_settings_form()
 */
function kmsuite_group_settings_page($form, $form_state) {

  $form['description'] = array(
    '#markup' => '<div>' . t('Group Management:  The Group Management configurations determine detailed information for each group you elect to have working in your website.  Note:  API Counts are caches for 24 hours to avoid slow page loads..</a>', array('!suite' => url('google.com'), '!kmcontact' => url('google.com'))) . '</div>',
  );

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);

  if (isset($presets['subscription_plugin']['page'])) {
    $settings = $presets['subscription_plugin']['page'];
  }

  if (isset($presets['subscription_plugin']['groups']) && count($presets['subscription_plugin']['groups']) > 0 && strlen($presets['config']['token']) > 3) {

    $form['km_group_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Subscription Page Settings'),
      '#description' => t('Add additional settings to how the subscription page should look and function'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['km_group_settings']['success_msg'] = array(
      '#type' => 'textfield',
      '#title' => t('Success Message'),
      '#default_value' => isset($settings['success_msg']) ? $settings['success_msg'] : t('Settings Saved.'),
      '#description' => t('Define the message shown upon successful subscription'),
    );
    $form['km_group_settings']['error_msg'] = array(
      '#type' => 'textfield',
      '#title' => t('Error Message'),
      '#default_value' => isset($settings['error_msg']) ? $settings['error_msg'] : t('An error occurred.'),
      '#description' => t('Define the message shown upon an error in the subscription'),
    );

    $form['km_group_settings']['page_desc'] = array(
      '#type' => 'text_format',
      '#title' => t('Page Description'),
      '#description' => t('Description for the Subscription page'),
      '#default_value' => isset($settings['page_desc']['value']) ? $settings['page_desc']['value'] : '',
      '#format' => isset($settings['page_desc']['format']) ? $settings['page_desc']['format'] : NULL,
      '#weight' => 0,
    );

    /* Group Selection */

    $data = array();

    if (isset($presets['subscription_plugin']['groups'])) {

      $form['data_table'] = array(
        '#type' => 'fieldset',
        '#title' => t('Group Selection'),
        '#description' => t('You can choose and order the groups available for use in this block.  Note:  If an Alias is available, it will be used instead of the title, otherwise the default title will be used.'),
      );

      $form['data_table']['km_subscription_show'] = array(
        '#type' => 'select',
        '#title' => t('Show / Hide Lists'),
        '#description' => t('Select if you want to show or hide your lists to the user'),
        '#options' => array(0 => t('Show'), 1 => t('Hide')),
        '#default_value' => isset($settings['groups']['visibility']) ? $settings['groups']['visibility'] : 0,
      );

      foreach ($presets['subscription_plugin']['groups'] as $k => $v) {

        $data[$k] = array(
          'enabled' => isset($settings['groups'][$k]['enabled']) ? $settings['groups'][$k]['enabled'] : FALSE,
          'weight' => isset($settings['groups'][$k]['weight']) ? $settings['groups'][$k]['weight'] : 0,
          'name' => $v['origname'],
          'alias' => $v['alias'],
        );
      }

      // Sort the rows.
      uasort($data, 'array_uasort');

      $group_table_data = _build_dragtable_help($data, 'data_table', array('enabled'));

      // Add the table to the form.
      $form['data_table']['table'] = array(
        '#theme' => 'table',
        'elements' => $group_table_data['row_elements'],
        '#header' => array(
          array('data' => NULL, 'colspan' => 2),
          t('Enabled'),
          t('Name'),
          t('Alias'),
        ),
        '#rows' => $group_table_data['rows'],
        '#empty' => t('There are no groups available.'),
        '#attributes' => array('id' => 'entry-order'),
      );

      drupal_add_tabledrag('entry-order', 'order', 'sibling', 'entry-order-weight');

    }
    /* /Group Selection */

    /* Fields Selection */

    if (isset($presets['subscription_plugin']['groups'])) {

      $form['field_table'] = array(
        '#type' => 'fieldset',
        '#title' => t('Field Selection'),
        '#description' => t('Optional fields are coming from the Field tab in the Subscription plugin.'),
      );

      foreach ($presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields'] as $k => $v) {

        $data1[$k] = array(
          'enabled' => isset($settings['fields'][$k]['enabled']) ? $settings['fields'][$k]['enabled'] : FALSE,
          'weight' => isset($settings['fields'][$k]['weight']) ? $settings['fields'][$k]['weight'] : 0,
          'name' => $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['names'][$k],
          'required' => isset($settings['fields'][$k]['required']) ? $settings['fields'][$k]['required'] : FALSE,
        );
      }

      // Sort the rows.
      uasort($data1, 'array_uasort');

      $field_table_data = _build_dragtable_help($data1, 'field_table', array('enabled', 'required'));

      // Add the table to the form.
      $form['field_table']['table'] = array(
        '#theme' => 'table',
        // The row form elements need to be processed and build,
        // therefore pass them as element children.
        'elements' => $field_table_data['row_elements'],
        '#header' => array(
          // We need two empty columns for the weigth field and the cross.
          array('data' => NULL, 'colspan' => 2),
          t('Enabled'),
          t('Required'),
          t('Name'),
        ),
        '#rows' => $field_table_data['rows'],
        '#empty' => t('There are no groups available.'),
        '#attributes' => array('id' => 'field-order'),
      );
      drupal_add_tabledrag('field-order', 'order', 'sibling', 'field-order-weight');
    }
    /* /Field Selection */

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Save Configuration',
      '#submit' => array('kmsuite_group_settings_page_submit'),
    );

  }
  else {
    drupal_set_message(t('Error:  Either Subscription List does not set or you do not have a proper token configured.'), 'error');
  }

  return $form;
}

/**
 * Custom submit handler for the config settings.
 *
 * @todo Ensure group origname is available.  Currently only works on second submit.
 */
function kmsuite_group_settings_page_submit($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);

  // Save the page specific data.
  $presets['subscription_plugin']['page']['success_msg'] = $form_state['values']['success_msg'];
  $presets['subscription_plugin']['page']['error_msg'] = $form_state['values']['error_msg'];
  $presets['subscription_plugin']['page']['page_desc'] = $form_state['values']['page_desc'];
  $presets['subscription_plugin']['page']['fields'] = $form_state['values']['field_table'];
  $presets['subscription_plugin']['page']['groups'] = $form_state['values']['data_table'];
  $presets['subscription_plugin']['page']['visibility'] = $form_state['values']['km_subscription_show'];

  variable_set('kmsuite_object', json_encode($presets));

  drupal_set_message(t('Configuration settings saved.'), 'status', FALSE);
}
