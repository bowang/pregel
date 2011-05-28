#ifndef MESSAGE_H
#define MESSAGE_H

#include "Common.h"

/*********************************
 *                               *
 *  Declaration of MessageList   *
 *                               *
 *********************************/

template <typename MessageValue>
class MessageList {
  public:
    MessageList();
    void addMessage(const MessageValue& message);
    void addMessage(const vector<MessageValue>& msgList);
    vector<MessageValue>* getMessages();
    bool empty();
    void clear();
  private:
    vector<MessageValue> messages;
    pthread_mutex_t mutex;
};

/**********************************
 *                                *
 * Declaration of MessageIterator *
 *                                *
 **********************************/

template <typename MessageValue>
class MessageIterator {
  public: 
    MessageIterator(MessageList<MessageValue>& msgList);
    bool done();
    void next();
    bool empty();
    MessageValue getValue();
  private:
    typename vector<MessageValue>::iterator itr;
    vector<MessageValue>* messages;
};

/*********************************
 *                               *
 * Implementation of MessageList *
 *                               *
 *********************************/

template <typename MessageValue>
MessageList<MessageValue>::MessageList () {
    pthread_mutex_init(&mutex, NULL);
    messages.clear();
    return;
}

template <typename MessageValue>
void MessageList<MessageValue>::addMessage(const MessageValue& message) {
    pthread_mutex_lock( &mutex );
    messages.push_back(message);
    pthread_mutex_unlock( &mutex );
    return;
}

template <typename MessageValue>
void MessageList<MessageValue>::addMessage(const vector<MessageValue>& msgList) {
    pthread_mutex_lock( &mutex );
    messages.insert(messages.end(), msgList.begin(), msgList.end());
    pthread_mutex_unlock( &mutex );
}

template <typename MessageValue>
vector<MessageValue>* MessageList<MessageValue>::getMessages() {
    return &messages;
}

template <typename MessageValue>
void MessageList<MessageValue>::clear() {
    messages.clear();
    return;
}

template <typename MessageValue>
bool MessageList<MessageValue>::empty() {
    return messages.empty();
}

/*************************************
 *                                   *
 * Implementation of MessageIterator *
 *                                   *
 *************************************/

template <typename MessageValue>
MessageIterator<MessageValue>::MessageIterator (MessageList<MessageValue>& msgList) {
    messages = msgList.getMessages();
    itr = messages->begin();
}

template <typename MessageValue>
bool MessageIterator<MessageValue>::done() {
    return (!(itr < messages->end()));
}

template <typename MessageValue>
bool MessageIterator<MessageValue>::empty() {
    return messages->empty();;
}

template <typename MessageValue>
void MessageIterator<MessageValue>::next() {
    itr++;
    return;
}

template <typename MessageValue>
MessageValue MessageIterator<MessageValue>::getValue() {
    return (*itr);
}

#endif
