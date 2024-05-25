#ifndef __HTML_BUILDER_H__
#define __HTML_BUILDER_H__

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>

namespace noname
{
    class HtmlBuilder;

    struct HtmlElement
    {
        std::string _name;
        std::string _text;
        std::vector<HtmlElement> _elements;
        const size_t indent_size = 2;

        HtmlElement() {}
        HtmlElement(const std::string &name, const std::string &text)
            : _name(name),
              _text(text)
        {
        }

        std::string str(int indent = 0) const
        {
            std::ostringstream oss;
            std::string i(indent_size * indent, ' ');
            oss << i << "<" << _name << ">" << std::endl;
            if (_text.size() > 0)
                oss << std::string(indent_size * (indent + 1), ' ') << _text << std::endl;

            for (const auto &e : _elements)
                oss << e.str(indent + 1);

            oss << i << "</" << _name << ">" << std::endl;
            return oss.str();
        }

        static std::unique_ptr<HtmlBuilder> build(std::string root_name)
        {
            return std::make_unique<HtmlBuilder>(root_name);
        }
    };

    class HtmlBuilder
    {
    public:
        HtmlBuilder(std::string root_name)
        {
            root._name = root_name;
        }

        HtmlBuilder(std::string name, std::string text)
        {
            root._name = name;
            root._text = text;
        }

        // void to start with
        HtmlBuilder &add_child(std::string child_name, std::string child_text)
        {
            HtmlElement e{child_name, child_text};
            root._elements.emplace_back(e);
            return *this;
        }

        // pointer based
        HtmlBuilder *add_child_2(std::string child_name, std::string child_text)
        {
            HtmlElement e{child_name, child_text};
            root._elements.emplace_back(e);
            return this;
        }

        std::string str() { return root.str(); }

        operator HtmlElement() const { return root; }

    private:
        HtmlElement root;
    };

    // int demo()
    // {
    //     // easier
    //     HtmlBuilder builder{"ul"};
    //     builder.add_child("li", "hello").add_child("li", "world");
    //     std::cout << builder.str() << std::endl;

    //     auto builder2 = HtmlElement::build("ul")
    //                         ->add_child_2("li", "hello")
    //                         ->add_child_2("li", "world");
    //     std::cout << builder2 << std::endl;

    //     getchar();
    //     return 0;
    // }
}
#endif // __HTML_BUILDER_H__