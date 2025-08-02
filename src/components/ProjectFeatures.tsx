import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { 
  Smartphone, 
  Laptop, 
  Cpu, 
  Shield, 
  Code, 
  Users,
  Zap,
  GitBranch,
  Settings
} from "lucide-react";

const features = [
  {
    icon: Smartphone,
    title: "ARM Device Support",
    description: "Native UEFI implementation for ARM-based mobile devices including Surface Duo and various smartphones."
  },
  {
    icon: Laptop,
    title: "Windows 11 ARM64",
    description: "Full Windows 10/11 ARM64 compatibility with proper UEFI boot support on mobile hardware."
  },
  {
    icon: Cpu,
    title: "Project Mu Based",
    description: "Built on Microsoft's Project Mu framework for modern, secure, and maintainable UEFI firmware."
  },
  {
    icon: Shield,
    title: "Secure Boot",
    description: "Implements secure boot protocols and modern security features for mobile UEFI environments."
  },
  {
    icon: Code,
    title: "Open Source",
    description: "Fully open source implementation with comprehensive documentation and community contributions."
  },
  {
    icon: Users,
    title: "Active Community",
    description: "Growing community of developers and enthusiasts pushing the boundaries of mobile computing."
  }
];

const achievements = [
  {
    icon: Zap,
    title: "Surface Duo Success",
    description: "Successfully booting Windows 11 on Surface Duo devices with full touch support."
  },
  {
    icon: GitBranch,
    title: "Multiple Devices",
    description: "Support for various ARM devices with ongoing expansion to new hardware."
  },
  {
    icon: Settings,
    title: "Hardware Integration",
    description: "Advanced hardware abstraction layer for seamless Windows-mobile integration."
  }
];

const ProjectFeatures = () => {
  return (
    <section className="py-24 bg-background">
      <div className="container mx-auto px-6">
        {/* Features Grid */}
        <div className="text-center mb-16">
          <h2 className="text-3xl lg:text-4xl font-bold mb-4">
            Revolutionary
            <span className="bg-gradient-primary bg-clip-text text-transparent"> Features</span>
          </h2>
          <p className="text-xl text-muted-foreground max-w-3xl mx-auto">
            Project Silicium brings desktop-class UEFI functionality to mobile devices, 
            enabling unprecedented computing possibilities.
          </p>
        </div>

        <div className="grid md:grid-cols-2 lg:grid-cols-3 gap-8 mb-20">
          {features.map((feature, index) => (
            <Card key={index} className="bg-gradient-card border-border/50 hover:border-primary/50 transition-all duration-300 hover:shadow-elegant group">
              <CardHeader>
                <div className="w-12 h-12 bg-primary/10 rounded-lg flex items-center justify-center mb-4 group-hover:bg-primary/20 transition-colors">
                  <feature.icon className="h-6 w-6 text-primary" />
                </div>
                <CardTitle className="text-lg group-hover:text-primary transition-colors">
                  {feature.title}
                </CardTitle>
              </CardHeader>
              <CardContent>
                <CardDescription className="text-muted-foreground leading-relaxed">
                  {feature.description}
                </CardDescription>
              </CardContent>
            </Card>
          ))}
        </div>

        {/* Achievements Section */}
        <div className="bg-card/50 backdrop-blur-sm rounded-2xl p-8 border border-border/50">
          <div className="text-center mb-12">
            <h3 className="text-2xl lg:text-3xl font-bold mb-4">Key Achievements</h3>
            <p className="text-muted-foreground max-w-2xl mx-auto">
              Milestones that showcase the potential of UEFI on mobile platforms.
            </p>
          </div>

          <div className="grid md:grid-cols-3 gap-8">
            {achievements.map((achievement, index) => (
              <div key={index} className="text-center group">
                <div className="w-16 h-16 bg-gradient-primary rounded-full flex items-center justify-center mx-auto mb-4 group-hover:shadow-glow transition-all duration-300">
                  <achievement.icon className="h-8 w-8 text-primary-foreground" />
                </div>
                <h4 className="text-lg font-semibold mb-2 group-hover:text-primary transition-colors">
                  {achievement.title}
                </h4>
                <p className="text-muted-foreground text-sm leading-relaxed">
                  {achievement.description}
                </p>
              </div>
            ))}
          </div>
        </div>
      </div>
    </section>
  );
};

export default ProjectFeatures;