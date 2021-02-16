#pragma once

#include <unordered_map>
#include <string>
#include <typeinfo>
#include <stdexcept>

namespace {

    class PropertyMap {
    public:
        PropertyMap(PropertyMap* parent = nullptr)
            : parent_(parent)
        {
        }

        template<typename T>
        void export_property(const std::string& name, const T& property) {
            if (find_property(name) || find_mutable_property(name)) {
                throw std::runtime_error("Property has already been exported");
            }

            properties_.insert(std::make_pair(name, Property {
                    typeid(T),
                    &property
            }));
        }

        template<typename T>
        void export_mutable_property(const std::string& name, T& property) {
            if (find_property(name) || find_mutable_property(name)) {
                throw std::runtime_error("Property has already been exported");
            }

            mutable_properties_.insert(std::make_pair(name, MutableProperty {
                    typeid(T),
                    &property
            }));
        }

        template<typename T>
        const T& import_property(const std::string& name) {
            if (Property* property = find_property(name)) {
                if (property->type != typeid(T)) {
                    throw std::runtime_error("Cannot import property as a different type");
                }

                return *reinterpret_cast<const T*>(property->data);
            }

            return import_mutable_property<T>(name);
        }

        template<typename T>
        T& import_mutable_property(const std::string& name) {
            if (MutableProperty* property = find_mutable_property(name)) {
                if (property->type != typeid(T)) {
                    throw std::runtime_error("Cannot import property as a different type");
                }

                return *reinterpret_cast<T*>(property->data);
            }

            if (find_property(name)) {
                throw std::runtime_error("Property is not mutable");
            }

            throw std::runtime_error("Failed to find property");
        }

    private:
        struct Property {
            const std::type_info& type;
            const void*           data;
        };

        struct MutableProperty {
            const std::type_info& type;
            void*                 data;
        };

        Property* find_property(const std::string& name) {
            if (auto it = properties_.find(name); it != properties_.end()) {
                return &it->second;
            }

            if (parent_) {
                return parent_->find_property(name);
            }

            return nullptr;
        }

        MutableProperty* find_mutable_property(const std::string& name) {
            if (auto it = mutable_properties_.find(name); it != mutable_properties_.end()) {
                return &it->second;
            }

            if (parent_) {
                return parent_->find_mutable_property(name);
            }

            return nullptr;
        }

        PropertyMap*                                     parent_;
        std::unordered_map<std::string, Property>        properties_;
        std::unordered_map<std::string, MutableProperty> mutable_properties_;
    };

}
