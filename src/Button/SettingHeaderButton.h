#pragma once

#include "Button.h"

class SettingHeaderButton : public Button {
  bool isActivated_m = false;

 public:
  SettingHeaderButton(Music& music, const std::string& name, float scale = 1.0,
                      float x = 0, float y = 0, bool origInCenter = true,
                      const MyConvexShape::HitboxType& hbType =
                          MyConvexShape::HitboxType::Rect);
  void deactivate();

  /// virtual
  void update(sf::Event& event, sf::Vector2f& mousePos) override;
  void activate() override;
  void reloadAfterOpenScene(sf::Vector2f& mousePos, bool wasReleased) override;

  bool isActivated() const;
};