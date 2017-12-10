#ifndef EE_UICUIBACKGROUND_HPP
#define EE_UICUIBACKGROUND_HPP

#include <eepp/ui/base.hpp>
#include <eepp/graphics/drawable.hpp>

namespace EE { namespace UI {

class UIControl;

class EE_API UIBackground {
	public:
		static UIBackground * New( UIControl * control );

		UIBackground( UIControl * control );

		~UIBackground();

		Color& getColor( const unsigned int& index = 0 );

		UIBackground * setColor( const Color& Col );

		const std::vector<Color>& getColors();

		UIBackground * setColors( const Color& TopLeftColor, const Color& BottomLeftColor, const Color& BottomRightColor, const Color& TopRightColor );

		UIBackground * setColorsTo( const Color& color );

		const BlendMode& getBlendMode() const;

		UIBackground * setBlendMode( const BlendMode& blend );

		const unsigned int& getCorners() const;

		UIBackground * setCorners( const unsigned int& corners );

		void draw( Rectf R, const Float& alpha );

		Drawable * getDrawable() const;

		void setDrawable( Drawable * drawable, bool ownIt );

	protected:
		std::vector<Color>	mColor;
		UIControl *			mControl;
		BlendMode		mBlendMode;
		unsigned int		mCorners;
		Drawable *			mDrawable;
		bool				mOwnIt;
};

}}

#endif
