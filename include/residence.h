class Residence
{
public:
  Residence();
  
  
private:
  unsigned int id;
  std::unordered_set<SolarPanel> solarPanels;
  
};
