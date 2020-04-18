#include <tgbot/tgbot.h>

void tgbot_main(const std::string& token) {
  TgBot::Bot bot(token);
  bot.getEvents().onCommand("start",
                            [&bot](TgBot::Message::Ptr message) {
    bot.getApi().sendMessage(message->chat->id, "Hi!");
  });
  bot.getEvents().onAnyMessage(
    [&bot](TgBot::Message::Ptr message) {
    printf("User wrote %s\n", message->text.c_str());
    if (StringTools::startsWith(message->text, "/start")) {
      return;
    }
    bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
  });
  try {
    printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
    TgBot::TgLongPoll longPoll(bot);
    while (true) {
      printf("Long poll started\n");
      longPoll.start();
    }
  } catch (TgBot::TgException& e) {
    printf("error: %s\n", e.what());
  }
}
