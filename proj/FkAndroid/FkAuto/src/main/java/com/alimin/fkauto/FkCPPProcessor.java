package com.alimin.fkauto;

import com.alimin.fkannotation.FkCPPSource;
import com.google.auto.service.AutoService;

import java.io.File;
import java.io.IOException;
import java.util.Collections;
import java.util.Set;

import javax.annotation.processing.AbstractProcessor;
import javax.annotation.processing.Messager;
import javax.annotation.processing.ProcessingEnvironment;
import javax.annotation.processing.RoundEnvironment;
import javax.lang.model.element.TypeElement;
import javax.tools.Diagnostic;

@AutoService(Process.class)
public class FkCPPProcessor extends AbstractProcessor {

    @Override
    public synchronized void init(ProcessingEnvironment processingEnvironment) {
        super.init(processingEnvironment);
        Messager messager = processingEnvironment.getMessager();
        String mRootPath = processingEnvironment.getOptions().getOrDefault("rootPath", ".");
        messager.printMessage(Diagnostic.Kind.NOTE, "mRootPath: " + mRootPath);
        File file = new File(mRootPath,System.currentTimeMillis() + ".txt");
        if (!file.exists()) {
            try {
                file.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    protected synchronized boolean isInitialized() {
        return super.isInitialized();
    }

    @Override
    public Set<String> getSupportedAnnotationTypes() {
        return Collections.singleton(FkCPPSource.class.getCanonicalName());
    }

    @Override
    public boolean process(Set<? extends TypeElement> set, RoundEnvironment roundEnvironment) {
        return true;
    }
}
