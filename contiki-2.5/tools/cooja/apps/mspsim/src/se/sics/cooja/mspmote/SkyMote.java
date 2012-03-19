/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: SkyMote.java,v 1.19 2010/09/09 19:56:59 nifi Exp $
 */

package se.sics.cooja.mspmote;

import java.io.File;

import org.apache.log4j.Logger;

import se.sics.cooja.Simulation;
import se.sics.cooja.mspmote.interfaces.CoojaM25P80;
import se.sics.cooja.mspmote.interfaces.SkyCoffeeFilesystem;
import se.sics.mspsim.platform.sky.SkyNode;

/**
 * @author Fredrik Osterlind
 */
public class SkyMote extends MspMote {
  private static Logger logger = Logger.getLogger(SkyMote.class);

  public SkyNode skyNode = null;

  public SkyMote() {
    super();
  }

  public SkyMote(MspMoteType moteType, Simulation sim) {
    super(moteType, sim);
  }

  protected boolean initEmulator(File fileELF) {
    try {
      skyNode = new SkyNode();
      registry = skyNode.getRegistry();
      skyNode.setFlash(new CoojaM25P80(skyNode.getCPU()));
      prepareMote(fileELF, skyNode);
    } catch (Exception e) {
      logger.fatal("Error when creating Sky mote: ", e);
      return false;
    }
    return true;
  }

  public void idUpdated(int newID) {
    skyNode.setNodeID(newID);
  }

  public SkyCoffeeFilesystem getFilesystem() {
    return getInterfaces().getInterfaceOfType(SkyCoffeeFilesystem.class);
  }

  public String toString() {
    return "Sky " + getID();
  }

}
