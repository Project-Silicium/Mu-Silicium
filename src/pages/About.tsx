import React from "react";
import { Link } from "react-router-dom";
import { Button } from "@/components/ui/button";
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Badge } from "@/components/ui/badge";
import { TooltipTerm } from '@/components/ui/tooltip-term';
import { 
  Github, 
  MessageCircle, 
  Cpu, 
  Smartphone, 
  Users, 
  Zap,
  ArrowDown,
  ExternalLink,
  Heart,
  Code,
  Shield,
  Layers
} from "lucide-react";

const About = () => {
  const scrollToAbout = () => {
    document.getElementById('about-content')?.scrollIntoView({ 
      behavior: 'smooth' 
    });
  };

  const features = [
    {
      icon: Cpu,
      title: "UEFI Booting",
      description: "Modern UEFI firmware implementation enabling advanced boot capabilities on mobile devices."
    },
    {
      icon: Smartphone,
      title: "Windows on Android", 
      description: "Run full Windows 10/11 ARM64 on your smartphone or tablet with native hardware support."
    },
    {
      icon: Users,
      title: "Community-Driven Support",
      description: "Active community of developers and enthusiasts contributing to device compatibility."
    },
    {
      icon: Layers,
      title: "Multi-Device Compatibility",
      description: "Support for various ARM-based devices across different manufacturers and chipsets."
    }
  ];

  const stats = [
    { number: "40+", label: "Supported Devices" },
    { number: "239", label: "Github Stars" },
    { number: "103", label: "Forks" },
    { number: "50+", label: "Contributors" }
  ];

  return (
    <div className="min-h-screen bg-background">
      {/* Hero Section */}
      <section className="relative min-h-screen bg-gradient-hero flex items-center justify-center overflow-hidden">
        {/* Background Pattern */}
        <div className="absolute inset-0 opacity-10">
          <div className="absolute inset-0 bg-gradient-to-br from-primary/20 to-accent/20"></div>
          <div className="absolute top-1/4 left-1/4 w-64 h-64 bg-primary/10 rounded-full blur-3xl animate-pulse"></div>
          <div className="absolute bottom-1/4 right-1/4 w-96 h-96 bg-accent/10 rounded-full blur-3xl animate-pulse delay-1000"></div>
        </div>

        {/* Full Section Background Image */}
        <div 
          className="absolute inset-0 opacity-20 bg-no-repeat bg-cover"
          style={{
            backgroundImage: 'url(./silicon.png)',
            filter: 'blur(0.8px)',
            backgroundPosition: 'center 20%',
          }}
        ></div>

        <div className="container mx-auto px-6 relative z-10">
          <div className="text-center max-w-4xl mx-auto">
            {/* Badge */}
            <Badge variant="secondary" className="mb-6 px-4 py-2 text-sm bg-primary/10 text-primary border-primary/20">
              <Zap className="w-4 h-4 mr-2" />
              Open Source UEFI Project
            </Badge>

            {/* Title Text */}
            <h1 className="text-5xl md:text-7xl font-bold mb-6 leading-tight">
              Project{' '}
              <span className="text-gradient-primary animate-glow">
                Silicium
              </span>
            </h1>

            {/* Tagline */}
            <p className="text-xl md:text-2xl text-muted-foreground mb-8 leading-relaxed">
              "Bringing <TooltipTerm term="UEFI">UEFI</TooltipTerm> to Android and Beyond"
            </p>

            <p className="text-lg text-muted-foreground mb-12 max-w-2xl mx-auto">
              Unlock the full potential of your mobile device with desktop-class <TooltipTerm term="firmware">firmware</TooltipTerm>
            </p>

            {/* CTA Buttons */}
            <div className="flex flex-col sm:flex-row gap-4 justify-center mb-16">
              <Button 
                variant="hero" 
                size="lg" 
                onClick={scrollToAbout}
                className="group"
              >
                Learn More
                <ArrowDown className="ml-2 h-5 w-5 group-hover:animate-bounce" />
              </Button>
              <Button 
                variant="cta" 
                size="lg"
                onClick={() => window.open('https://github.com/Project-Silicium', '_blank')}
              >
                <Github className="mr-2 h-5 w-5" />
                View on GitHub
              </Button>
            </div>

            {/* Stats */}
            <div className="grid grid-cols-2 md:grid-cols-4 gap-8 max-w-2xl mx-auto">
              {stats.map((stat, index) => (
                <div key={index} className="text-center">
                  <div className="text-2xl md:text-3xl font-bold text-primary mb-1">
                    {stat.number}
                  </div>
                  <div className="text-sm text-muted-foreground">
                    {stat.label}
                  </div>
                </div>
              ))}
            </div>
          </div>
        </div>

        {/* Floating Image */}
        <div className="absolute bottom-10 right-10 opacity-20 animate-float hidden lg:block">
          <Smartphone className="w-32 h-32 text-primary" />
        </div>
      </section>

      {/* About Section */}
      <section id="about-content" className="py-24 bg-background">
        <div className="container mx-auto px-6">
          <div className="max-w-4xl mx-auto">
            <div className="text-center mb-16">
              <h2 className="text-3xl md:text-4xl font-bold mb-6">
                About 
                <span className="text-gradient-primary"> Project Silicium</span>
              </h2>
            </div>

            <Card className="bg-gradient-card border-border/50 hover:border-primary/50 transition-all duration-300 hover:shadow-elegant">
              <CardContent className="p-8 md:p-12">
                <div className="prose prose-lg prose-invert max-w-none">
                  <p className="text-lg leading-relaxed text-muted-foreground mb-6">
                    Project Silicium is a bold, community-driven open-source initiative that focuses on porting <TooltipTerm term="UEFI">UEFI (Unified Extensible Firmware Interface)</TooltipTerm> to Android-based smartphones and tablets. <TooltipTerm term="UEFI">UEFI</TooltipTerm> has long been a foundational standard in the world of PCs and servers, allowing powerful features such as <TooltipTerm term="secure boot">secure booting</TooltipTerm>, operating system flexibility, and deeper hardware-level control. Silicium brings this same level of control and flexibility to mobile devices, allowing users to go far beyond the limitations imposed by traditional Android <TooltipTerm term="bootloader">bootloaders</TooltipTerm>.
                  </p>
                  <p className="text-lg leading-relaxed text-muted-foreground mb-6">
                    The core mission of Silicium is to unlock the full potential of modern <TooltipTerm term="ARM">ARM-based</TooltipTerm> devices. By enabling them to boot alternative operating systems like <TooltipTerm term="Windows ARM64">Windows</TooltipTerm>, various Linux distributions, and even custom OSes, Project Silicium opens the door to an entirely new realm of mobile computing. Imagine turning your everyday smartphone into a full-fledged desktop environment, development machine, or dual-boot device — all while maintaining performance and functionality. This is no longer a concept of the future, but a reality being built today.
                  </p>
                  <p className="text-lg leading-relaxed text-muted-foreground">
                    What truly sets Project Silicium apart is its dedicated global community of developers, contributors, and enthusiasts. Together, they are continuously working to reverse-engineer, document, and develop critical low-level firmware components. This level of collaboration has made it possible to support a growing list of devices, with detailed guides, source code, and support channels open to all. Silicium isn’t just about booting a new OS — it’s about freedom, experimentation, and giving control back to the user.
                  </p>
                </div>
              </CardContent>
            </Card>
          </div>
        </div>
      </section>

      {/* Features Section */}
      <section className="py-24 bg-card/20">
        <div className="container mx-auto px-6">
          <div className="text-center mb-16">
            <h2 className="text-3xl md:text-4xl font-bold mb-4">
              Key 
              <span className="text-gradient-primary"> Features</span>
            </h2>
            <p className="text-xl text-muted-foreground max-w-2xl mx-auto">
              Discover what makes Project Silicium revolutionary
            </p>
          </div>

          <div className="grid md:grid-cols-2 lg:grid-cols-4 gap-8">
            {features.map((feature, index) => (
              <Card key={index} className="bg-gradient-card border-border/50 hover:border-primary/50 transition-all duration-300 hover:shadow-elegant group">
                <CardHeader className="text-center">
                  <div className="w-16 h-16 bg-primary/10 rounded-full flex items-center justify-center mx-auto mb-4 group-hover:bg-primary/20 transition-colors">
                    <feature.icon className="h-8 w-8 text-primary" />
                  </div>
                  <CardTitle className="text-xl group-hover:text-primary transition-colors">
                    {feature.title}
                  </CardTitle>
                </CardHeader>
                <CardContent>
                  <CardDescription className="text-center text-muted-foreground leading-relaxed">
                    {feature.description}
                  </CardDescription>
                </CardContent>
              </Card>
            ))}
          </div>
        </div>
      </section>

      {/* Supported Devices Section */}
      <section className="py-24 bg-background">
        <div className="container mx-auto px-6">
          <div className="text-center mb-16">
            <h2 className="text-3xl md:text-4xl font-bold mb-4">
              Supported 
              <span className="text-gradient-primary"> Devices</span>
            </h2>
            <p className="text-xl text-muted-foreground max-w-2xl mx-auto mb-8">
              Check compatibility for your device and see what's working
            </p>
          </div>

          <div className="max-w-2xl mx-auto">
            <Card className="bg-gradient-card border-border/50 text-center">
              <CardContent className="p-8">
                <Smartphone className="w-16 h-16 text-primary mx-auto mb-6" />
                <h3 className="text-2xl font-bold mb-4">Device Compatibility</h3>
                <p className="text-muted-foreground mb-6 leading-relaxed">
                  Explore our comprehensive device compatibility database with detailed 
                  status information for UEFI and Windows features.
                </p>
              <Link to="/devices">
  <Button variant="hero" size="lg" className="group">
    View All Devices
    <ExternalLink className="ml-2 h-5 w-5 group-hover:translate-x-1 transition-transform" />
  </Button>
</Link>
              </CardContent>
            </Card>
          </div>
        </div>
      </section>

      {/* Contribute Section */}
      <section className="py-24 bg-card/20">
        <div className="container mx-auto px-6">
          <div className="text-center mb-16">
            <h2 className="text-3xl md:text-4xl font-bold mb-4">
              Join Our 
              <span className="text-gradient-primary"> Community</span>
            </h2>
            <p className="text-xl text-muted-foreground max-w-2xl mx-auto">
              Help us bring UEFI to more devices and expand the possibilities of mobile computing
            </p>
          </div>

          <div className="max-w-4xl mx-auto">
            <Card className="bg-gradient-card border-border/50 hover:border-primary/50 transition-all duration-300 hover:shadow-elegant">
              <CardContent className="p-8 md:p-12">
                <div className="grid md:grid-cols-2 gap-8 items-center">
                  <div>
                    <div className="flex items-center gap-3 mb-6">
                      <Heart className="h-8 w-8 text-red-400" />
                      <h3 className="text-2xl font-bold">Contribute to Silicium</h3>
                    </div>
                    <p className="text-muted-foreground mb-6 leading-relaxed">
                      Whether you're a developer, tester, or documentation writer, there's a place 
                      for you in our community. Help us make UEFI accessible to everyone.
                    </p>
                    <div className="flex items-center gap-2 text-sm text-muted-foreground mb-6">
                      <Code className="h-4 w-4" />
                      <span>Open Source • MIT License • Community Driven</span>
                    </div>
                  </div>
                  
                  <div className="space-y-4">
                    <Button 
                      variant="hero" 
                      size="lg" 
                      className="w-full group"
                      onClick={() => window.open('https://github.com/Project-Silicium', '_blank')}
                    >
                      <Github className="mr-2 h-5 w-5 group-hover:animate-spin" />
                      Contribute on GitHub
                    </Button>
                    <Button 
                      variant="cta" 
                      size="lg" 
                      className="w-full"
                      onClick={() => window.open('https://discord.com/invite/Dx2QgMx7Sv', '_blank')}
                    >
                      <MessageCircle className="mr-2 h-5 w-5" />
                      Join Discord Community
                    </Button>
                  </div>
                </div>
              </CardContent>
            </Card>
          </div>
        </div>
      </section>
    </div>
  );
};

export default About;

