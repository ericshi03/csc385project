#pragma once

void init();
void disconnect();
void publish_message(const char* topic, const char* message);   
bool subscribe_to_topic(const char* topic);