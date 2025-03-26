#pragma once
#include <sequence-cpp/sequence.hpp>

namespace sequence{
    namespace interfaces{
        enum class ValueType{
            NONE,
            NUMBER,
            STRING,
            SEQUENCE,
            SEQUENCES,
        };
        inline std::string value_type_name(ValueType type){
            switch(type){
                case ValueType::NONE: return "NONE";
                case ValueType::NUMBER: return "NUMBER";
                case ValueType::STRING: return "STRING";
                case ValueType::SEQUENCE: return "SEQUENCE";
                case ValueType::SEQUENCES: return "SEQUENCES";
            }
            return "UNKNOWN";
        }
        class Value{
        public:
            Value(): type(ValueType::NONE) {}
            Value(double value): type(ValueType::NUMBER), number(value) {}
            Value(const std::string& value): type(ValueType::STRING), string(value) {}
            Value(const Sequence::ptr& value): type(ValueType::SEQUENCE), sequence(value) {}
            Value(const Sequence::list& value): type(ValueType::SEQUENCES), sequences(value) {}

            ValueType get_type() const { return type; }

            template<ValueType T>
            typename std::enable_if<T == ValueType::NUMBER, double>::type get() const {
                return number;
            }
            template<ValueType T>
            typename std::enable_if<T == ValueType::STRING, const std::string&>::type get() const {
                return string;
            }
            template<ValueType T>
            typename std::enable_if<T == ValueType::SEQUENCE, const Sequence::ptr&>::type get() const {
                return sequence;
            }
            template<ValueType T>
            typename std::enable_if<T == ValueType::SEQUENCES, const Sequence::list&>::type get() const {
                return sequences;
            }
        private:
            ValueType type;
            double number;
            std::string string;
            Sequence::ptr sequence;
            Sequence::list sequences;
        };
    }
}