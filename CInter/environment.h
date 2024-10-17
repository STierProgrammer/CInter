#pragma once

#include "values.h"

#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <memory>
#include <string>

class Environment {
	private:
		std::shared_ptr<Environment> parent;
		std::map<std::string, std::shared_ptr<RuntimeValue>> variables;
		std::set<std::string> constants;
	
	public:
		Environment(std::shared_ptr<Environment> parentENV = nullptr) : parent(parentENV) {}
        std::shared_ptr<RuntimeValue> declareVariable(const std::string& varname, std::shared_ptr<RuntimeValue> value, bool constant) {
            if (variables.find(varname) != variables.end()) {
                throw std::runtime_error("Cannot declare variable " + varname + ". As it already is defined.");
            }

            variables[varname] = value;

            if (constant) {
                constants.insert(varname);
            }

            return value;
        }

        std::shared_ptr<RuntimeValue> assignVariable(const std::string& varname, const std::shared_ptr<RuntimeValue> value) {
            Environment* env = resolve(varname);

            if (env->constants.find(varname) != env->constants.end()) {
                throw std::runtime_error("Cannot reassign variable " + varname + " as it was declared constant.");
            }

            env->variables[varname] = value;
            return value;
        }

        std::shared_ptr<RuntimeValue> lookupVariable(const std::string& varname) {
            Environment* env = resolve(varname);
            auto it = env->variables.find(varname);
            if (it == env->variables.end()) {
                throw std::runtime_error("Cannot find variable " + varname);
            }
            return it->second;
        }

    private:
        Environment* resolve(const std::string& varname) {
            if (variables.find(varname) != variables.end()) {
                return this;
            }

            if (parent) {  
                return parent->resolve(varname);
            }

            throw std::runtime_error("Cannot resolve '" + varname + "' as it does not exist.");
        }

};