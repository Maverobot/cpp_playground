#include <iostream>
#include <memory>
#include <sstream>
#include <string>

/*
 * The Builder Pattern decouples the creation of the object from the object
 * itself. The main idea behind is that an object does not have to be
 * responsible for its own creation. The correct and valid assembly of a complex
 * object may be a complicated task in itself, so this task can be delegated to
 * another class.
 */

class EmailBuilder;

class Email {
 public:
  // Definition is below after the definition of EmailBuilder
  static EmailBuilder build();

  std::string str() const {
    std::stringstream stream;
    stream << "from: " << from_ << "\nto: " << to_ << "\nsubject: " << subject_
           << "\nbody: " << body_;
    return stream.str();
  }

 private:
  // Restrict construction only to the builder.
  Email() = default;
  friend EmailBuilder;
  std::string from_;
  std::string to_;
  std::string subject_;
  std::string body_;
};

class EmailBuilder {
 public:
  EmailBuilder& from(std::string from) noexcept {
    email_.from_ = from;
    return *this;
  }
  EmailBuilder& to(std::string to) noexcept {
    email_.to_ = to;
    return *this;
  }
  EmailBuilder& subject(std::string subject) noexcept {
    email_.subject_ = subject;
    return *this;
  }
  EmailBuilder& body(std::string body) noexcept {
    email_.body_ = body;
    return *this;
  }
  operator Email&&() {
    return std::move(email_);  // notice the move
  }

 private:
  Email email_;
};

EmailBuilder Email::build() {
  return EmailBuilder();
}

int main(int argc, char* argv[]) {
  Email email = Email::build()
                    .from("sender")
                    .to("receiver")
                    .subject("unimportant letter")
                    .body("This is an example for builder pattern.");

  std::cout << email.str() << std::endl;
  return 0;
}
