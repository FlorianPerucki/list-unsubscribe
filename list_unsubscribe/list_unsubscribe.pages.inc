<?php
/**
 * @file
 * Defines form and page callbacks for the list-unsubscribe module.
 */

/** 
 * Form submit and page callback for unsubscribing an email address
 */
function list_unsubscribe_unsubscribe_email($email){
  if(!filter_var($email, FILTER_VALIDATE_EMAIL)) {
    drupal_set_message(t("Invalid email address.", "error"));
    drupal_goto('list-unsubscribe');
  }
  try {
    $query = db_insert('list_unsubscribe')
      ->fields(array(
      'email' => $email,
      'timestamp' => REQUEST_TIME,
    ))
      ->execute();
  }
  catch(Exception $e) {
    drupal_set_message(t('This email address was already unsubscribed'));
    drupal_goto('<front>');
  }
  drupal_set_message(t("You will not receive any more email from us."));
  drupal_goto();
}

/** 
 * Form callback for unsubscribing an email address
 */
function list_unsubscribe_unsubscribe_email_form($form, $form_state){
  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#size' => 60,
    '#required' => TRUE,
  );
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Stop emailing me!'),
  );
  return $form;
}

/** 
 * Form submit callback for unsubscribing an email address form
 */
function list_unsubscribe_unsubscribe_email_form_submit($form, $form_state){
  list_unsubscribe_unsubscribe_email($form_state['input']['mail']);
}

/** 
 * Helper function that checks if an email address was unsubscribed
 */
function _list_unsubscribe_email_check($email){
  if(!filter_var($email, FILTER_VALIDATE_EMAIL)) {      
    $query = db_select('unsubscribemail', 'unsub');
    $query->addField('unsub', 'email', 'email');
    $query->condition('unsub.email', $email);
    $query = $query->execute();
    return $query->fetchField() != '';
  }
  return FALSE;
}
