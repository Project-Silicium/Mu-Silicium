import { useState } from "react";
import { 
  Home, 
  Info, 
  BookOpen, 
  Users, 
  Cpu,
  Smartphone 
} from "lucide-react";
import { Link, useLocation } from "react-router-dom";

import {
  Sidebar,
  SidebarContent,
  SidebarGroup,
  SidebarGroupContent,
  SidebarGroupLabel,
  SidebarMenu,
  SidebarMenuButton,
  SidebarMenuItem,
  SidebarTrigger,
  useSidebar,
} from "@/components/ui/sidebar";

const mainItems = [
  { title: "Home", url: "/", icon: Home },
  { title: "About", url: "/about", icon: Info },
  { title: "Features", url: "/features", icon: Cpu },
  { title: "Status", url: "/devices", icon: Smartphone },
];

const resourceItems = [
  { title: "Guides", url: "/guides", icon: BookOpen },
  { title: "Contribute", url: "/contribute", icon: Users },
];

export function AppSidebar() {
  const { state } = useSidebar();
const location = useLocation();
const isActive = (path: string) => location.pathname === path;
  const getNavLinkCls = (path: string) =>
  isActive(path)
    ? "bg-primary text-primary-foreground font-medium"
    : "hover:bg-accent hover:text-accent-foreground text-muted-foreground hover:text-foreground transition-colors";
  return (
    <Sidebar
      collapsible="icon"
      className="border-r border-border"
    >
      <SidebarContent className="bg-card">
        {/* Header */}
        <div className="p-4 border-b border-border">
          <div className="flex items-center gap-2">
            <div className="w-8 h-8 bg-gradient-primary rounded-lg flex items-center justify-center">
              <Cpu className="h-4 w-4 text-primary-foreground" />
            </div>
            <div className="group-data-[collapsible=icon]:hidden">
              <h2 className="font-bold text-sm text-white">Project Silicium</h2>
              <p className="text-xs text-muted-foreground">UEFI on Mobile</p>
            </div>
          </div>
        </div>

        {/* Main Navigation */}
        <SidebarGroup>
          <SidebarGroupLabel className="text-white">Navigation</SidebarGroupLabel>
          <SidebarGroupContent>
            <SidebarMenu>
              {mainItems.map((item) => (
                <SidebarMenuItem key={item.title}>
                  <SidebarMenuButton asChild>
                    <Link to={item.url} className={getNavLinkCls(item.url)}>
                      <item.icon className="h-4 w-4" />
                      <span className="group-data-[collapsible=icon]:hidden">{item.title}</span>
                    </Link>
                  </SidebarMenuButton>
                </SidebarMenuItem>
              ))}
            </SidebarMenu>
          </SidebarGroupContent>
        </SidebarGroup>

        {/* Resources */}
        <SidebarGroup>
          <SidebarGroupLabel className="text-white">Resources</SidebarGroupLabel>
          <SidebarGroupContent>
            <SidebarMenu>
              {resourceItems.map((item) => (
                <SidebarMenuItem key={item.title}>
                  <SidebarMenuButton asChild>
                    <Link to={item.url} className={getNavLinkCls(item.url)}>
                      <item.icon className="h-4 w-4" />
                      <span className="group-data-[collapsible=icon]:hidden">{item.title}</span>
                    </Link>
                  </SidebarMenuButton>
                </SidebarMenuItem>
              ))}
            </SidebarMenu>
          </SidebarGroupContent>
        </SidebarGroup>
      </SidebarContent>
    </Sidebar>
  );
}

