import React, { useState } from "react";
import { Link } from "react-router-dom";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import { Badge } from "@/components/ui/badge";
import { Collapsible, CollapsibleContent, CollapsibleTrigger } from "@/components/ui/collapsible";
import { ChevronDown, ChevronRight, BookOpen, Monitor, Smartphone, Cpu, Settings, FileText, ExternalLink } from "lucide-react";

const guidesSections = [
  {
    id: "general",
    title: "General Guides",
    icon: <Monitor className="h-5 w-5" />,
    description: "This Section contains Guides like: Installing Windows or Linux.",
    status: "available",
    subsections: [
      { name: "Installing Windows", status: "available" },
      { name: "Installing Linux", status: "available" },
      { name: "Basic Setup", status: "available" }
    ]
  },
  {
    id: "vendor",
    title: "Vendor Guides", 
    icon: <Settings className="h-5 w-5" />,
    description: "This Section contains Guides for Vendor Specific Guides.",
    status: "available",
    subsections: [
      { name: "Samsung Specific", status: "available" },
      { name: "OnePlus Specific", status: "available" },
      { name: "Xiaomi Specific", status: "available" }
    ]
  },
  {
    id: "device",
    title: "Device Guides",
    icon: <Smartphone className="h-5 w-5" />,
    description: "Device-specific installation and setup guides.",
    status: "coming-soon",
    subsections: []
  },
  {
    id: "exynos", 
    title: "Exynos Guides",
    icon: <Cpu className="h-5 w-5" />,
    description: "Guides specific to Samsung Exynos processors.",
    status: "coming-soon",
    subsections: []
  },
  {
    id: "snapdragon",
    title: "Snapdragon Guides",
    icon: <Cpu className="h-5 w-5" />,
    description: "This Section contains Guides like how to use Mass Storage in TWRP.",
    status: "available",
    subsections: [
      { name: "Mass Storage in TWRP", status: "available" },
      { name: "Fastboot Commands", status: "available" },
      { name: "Recovery Setup", status: "available" }
    ]
  },
  {
    id: "tegra",
    title: "Tegra Guides", 
    icon: <Cpu className="h-5 w-5" />,
    description: "Guides for NVIDIA Tegra processors.",
    status: "coming-soon",
    subsections: []
  },
  {
    id: "silicium",
    title: "Silicium Guides",
    icon: <FileText className="h-5 w-5" />,
    description: "This Section Contains Guides like How to Port a Device and/or SoC.",
    status: "available",
    subsections: [
      { name: "How to port a Snapdragon device", status: "available", link: "/guides/snapdragon-porting" },
      { name: "How to Port a SoC", status: "available" },
      { name: "Development Setup", status: "available" }
    ]
  }
];

const statusConfig = {
  available: { symbol: "✅", label: "Available", color: "bg-green-500/20 text-green-400 border-green-500/30" },
  "coming-soon": { symbol: "🚧", label: "Coming Soon", color: "bg-yellow-500/20 text-yellow-400 border-yellow-500/30" }
};

export default function Guides() {
  const [openSections, setOpenSections] = useState<Set<string>>(new Set());

  const toggleSection = (sectionId: string) => {
    const newOpenSections = new Set(openSections);
    if (newOpenSections.has(sectionId)) {
      newOpenSections.delete(sectionId);
    } else {
      newOpenSections.add(sectionId);
    }
    setOpenSections(newOpenSections);
  };

  return (
    <div className="min-h-screen bg-background p-4 md:p-6">
      <div className="max-w-6xl mx-auto space-y-6">
        {/* Header */}
        <div className="text-center space-y-4">
          <div className="flex items-center justify-center gap-3">
            <BookOpen className="h-10 w-10 text-primary" />
            <h1 className="text-3xl md:text-4xl font-bold bg-gradient-to-r from-primary to-primary/60 bg-clip-text text-transparent">
              Project Silicium Guides
            </h1>
          </div>
          <p className="text-muted-foreground text-lg max-w-3xl mx-auto">
            Comprehensive guides for UEFI development, device porting, and operating system installation on ARM devices.
            Learn how to bring desktop operating systems to mobile hardware.
          </p>
          <div className="flex items-center justify-center gap-2 text-sm text-muted-foreground">
            <ExternalLink className="h-4 w-4" />
            <span>Source: Project-Silicium/Guides Repository</span>
          </div>
        </div>

        {/* Quick Stats */}
        <Card className="backdrop-blur-sm bg-card/50 border-border/50">
          <CardContent className="p-6">
            <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
              <div className="text-center">
                <div className="text-2xl font-bold text-primary">
                  {guidesSections.filter(s => s.status === "available").length}
                </div>
                <div className="text-sm text-muted-foreground">Available Sections</div>
              </div>
              <div className="text-center">
                <div className="text-2xl font-bold text-primary">
                  {guidesSections.reduce((acc, section) => acc + section.subsections.length, 0)}
                </div>
                <div className="text-sm text-muted-foreground">Total Guides</div>
              </div>
              <div className="text-center">
                <div className="text-2xl font-bold text-primary">
                  {guidesSections.filter(s => s.status === "coming-soon").length}
                </div>
                <div className="text-sm text-muted-foreground">Coming Soon</div>
              </div>
            </div>
          </CardContent>
        </Card>

        {/* Guide Sections */}
        <div className="space-y-4">
          {guidesSections.map((section) => (
            <Card key={section.id} className="backdrop-blur-sm bg-card/50 border-border/50 overflow-hidden">
              <Collapsible 
                open={openSections.has(section.id)}
                onOpenChange={() => toggleSection(section.id)}
              >
                <CollapsibleTrigger asChild>
                  <CardHeader className="cursor-pointer hover:bg-muted/50 transition-colors p-6">
                    <div className="flex items-center justify-between">
                      <div className="flex items-center gap-4">
                        <div className="text-primary">
                          {section.icon}
                        </div>
                        <div className="text-left">
                          <CardTitle className="text-xl flex items-center gap-3">
                            {section.title}
                            <Badge 
                              variant="secondary" 
                              className={`${statusConfig[section.status].color} border text-xs`}
                            >
                              <span className="mr-1">{statusConfig[section.status].symbol}</span>
                              {statusConfig[section.status].label}
                            </Badge>
                          </CardTitle>
                          <p className="text-sm text-muted-foreground mt-1">
                            {section.description}
                          </p>
                        </div>
                      </div>
                      <div className="flex items-center gap-2">
                        {section.subsections.length > 0 && (
                          <span className="text-sm text-muted-foreground">
                            {section.subsections.length} guide{section.subsections.length !== 1 ? 's' : ''}
                          </span>
                        )}
                        {openSections.has(section.id) ? 
                          <ChevronDown className="h-4 w-4" /> : 
                          <ChevronRight className="h-4 w-4" />
                        }
                      </div>
                    </div>
                  </CardHeader>
                </CollapsibleTrigger>

                {section.subsections.length > 0 && (
                  <CollapsibleContent>
                    <CardContent className="pt-0 px-6 pb-6">
                      <div className="grid gap-3">
                        {section.subsections.map((subsection, index) => (
                          <Card key={index} className="bg-background/40 border-border/40 hover:bg-muted/20 transition-colors cursor-pointer">
                            <CardContent className="p-4">
                              <div className="flex items-center justify-between">
                                <div className="flex items-center gap-3">
                                  <FileText className="h-4 w-4 text-muted-foreground" />
                                  {subsection.link ? (
                                    <Link 
                                      to={subsection.link}
                                      className="font-medium text-primary hover:underline"
                                    >
                                      {subsection.name}
                                    </Link>
                                  ) : (
                                    <span className="font-medium">{subsection.name}</span>
                                  )}
                                </div>
                                <div className="flex items-center gap-2">
                                  <Badge 
                                    variant="secondary" 
                                    className="bg-green-500/20 text-green-400 border-green-500/30 text-xs"
                                  >
                                    ✅ Available
                                  </Badge>
                                  <ExternalLink className="h-4 w-4 text-muted-foreground" />
                                </div>
                              </div>
                            </CardContent>
                          </Card>
                        ))}
                      </div>
                    </CardContent>
                  </CollapsibleContent>
                )}

                {section.subsections.length === 0 && section.status === "coming-soon" && (
                  <CollapsibleContent>
                    <CardContent className="pt-0 px-6 pb-6">
                      <Card className="bg-background/40 border-border/40">
                        <CardContent className="p-4">
                          <div className="text-center text-muted-foreground">
                            <FileText className="h-8 w-8 mx-auto mb-2 opacity-50" />
                            <p>No guides available yet.</p>
                            <p className="text-sm">Check back soon for updates!</p>
                          </div>
                        </CardContent>
                      </Card>
                    </CardContent>
                  </CollapsibleContent>
                )}
              </Collapsible>
            </Card>
          ))}
        </div>

        {/* Footer Note */}
        <Card className="backdrop-blur-sm bg-card/50 border-border/50">
          <CardContent className="p-6 text-center">
            <div className="space-y-2">
              <p className="text-sm text-muted-foreground">
                These guides are maintained by the Project Silicium community.
              </p>
              <p className="text-sm text-muted-foreground">
                For the latest updates and contributions, visit the{" "}
                <a 
                  href="https://github.com/Project-Silicium/Guides" 
                  target="_blank" 
                  rel="noopener noreferrer"
                  className="text-primary hover:underline inline-flex items-center gap-1"
                >
                  GitHub repository
                  <ExternalLink className="h-3 w-3" />
                </a>
              </p>
            </div>
          </CardContent>
        </Card>
      </div>
    </div>
  );
}
