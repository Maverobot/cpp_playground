#include <iostream>
#include <string>

/**
 * Not that this is not a design pattern but an implementation pattern as it does not reduce
 * coupling or have any architectural effect.
 */

class Email {
 public:
  struct From {
    std::string address;
  };

  struct To {
    std::string address;
  };

  struct Subject {
    std::string content;
  };

  struct Body {
    std::string content;
  };

  template <typename... Args>
  Email(Args&&... args) {
    (set(args), ...);
  }

  void set(const From& from) { from_ = from; }
  void set(const To& to) { to_ = to; }
  void set(const Subject& subject) { subject_ = subject; }
  void set(const Body& body) { body_ = body; }

  std::string toString() const {
    return "From: " + from_.address + " To: " + to_.address + " Subject: " + subject_.content +
           " Body: " + body_.content;
  }

 private:
  From from_;
  To to_;
  Subject subject_;
  Body body_;
};

int main(int argc, char* argv[]) {
  {
    Email email(Email::From{"abc@gmail.com"}, Email::To{"xyz@gmail.com"});
    std::cout << "Email constructed: \n\t" + email.toString() << "\n";
  }

  {
    Email email(Email::Subject{"Some random topic"}, Email::To{"xyz@gmail.com"},
                Email::Body{"Hi, nothing important."}, Email::From{"abc@gmail.com"});
    std::cout << "Email constructed: \n\t" + email.toString() << "\n";
  }
  return 0;
}
