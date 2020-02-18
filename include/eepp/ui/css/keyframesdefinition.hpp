#ifndef EE_UI_CSS_KEYFRAMESDEFINITION_HPP
#define EE_UI_CSS_KEYFRAMESDEFINITION_HPP

#include <eepp/math/ease.hpp>
#include <eepp/system/time.hpp>
#include <eepp/ui/css/stylesheetproperty.hpp>
#include <eepp/ui/css/stylesheetstyle.hpp>
#include <map>
#include <vector>

using namespace EE::Math;
using namespace EE::System;

namespace EE { namespace UI { namespace CSS {

class EE_API KeyframesDefinition {
  public:
	struct KeyframeBlock {
		Float normalizedTime;
		StyleSheetProperties properties;
	};

	static KeyframesDefinition parseKeyframes( const std::string& name,
											   const StyleSheetStyleVector& keyframeBlocks );

	const std::string& getName() const;
	const std::map<Float, KeyframeBlock>& getKeyframeBlocks() const;

	std::string name;
	std::map<Float, KeyframeBlock> keyframeBlocks;
};

typedef std::map<std::string, KeyframesDefinition> KeyframesDefinitionMap;

}}} // namespace EE::UI::CSS

#endif // EE_UI_CSS_KEYFRAMESDEFINITION_HPP
