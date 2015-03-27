#ifndef YAML_CONVERTERS_H
#define YAML_CONVERTERS_H

#include <logging/Logger.h>
#include <system/IPCName.h>
#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<LevelId>
	{
		static Node encode (const LevelId& level)
		{
			std::string name = level_name (level);
			boost::algorithm::trim_right (name);
			Node node;
			node = name;
			return node;
		}

		static bool decode (const Node& node, LevelId& level)
		{
			try {
				std::string name = node.as<std::string> ();
				level = level_id (name);
				return true;
			} catch (TypedBadConversion<std::string>& e) {
				return false;
			}
		}
	};

	template<>
	struct convert<IPCName>
	{
		static Node encode (const IPCName& name)
		{
			Node node;
			node["path"] = name.path;
			node["index"] = name.index;
			return node;
		}

		static bool decode (const Node& node, IPCName& name)
		{
			if (!node.IsMap ()) {
				return false;
			}

			try {
				name.path = node["path"].as<std::string> ();
				name.index = node["index"].as<char> ();
				return true;
			} catch (std::exception& e) {
				return false;
			}
		}
	};
}

#endif	/* YAML_CONVERTERS_H */

